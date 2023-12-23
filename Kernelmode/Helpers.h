//ptr utils
template <typename Type>
_FI Type EPtr(Type Ptr) {
	auto Key = (ULONG64)SharedUserData->Cookie *
		SharedUserData->Cookie *
		SharedUserData->Cookie *
		SharedUserData->Cookie;
	return (Type)((ULONG64)Ptr ^ Key);
}

//kernel memory utils
_FI PVOID KAlloc(ULONG Size) {
	PVOID Buff = ImpCall(ExAllocatePoolWithTag, NonPagedPoolNx, Size, 'KgxD');
	MemZero(Buff, Size);
	return Buff;
}
_FI PVOID KAlloc2(ULONG64 Size) {
	PVOID Buff = ImpCall(ExAllocatePoolWithTag, NonPagedPoolNx, Size, 'KgxD');
	MemZero(Buff, Size);
	return Buff;
}

_FI void KFree(PVOID Ptr) {
	ImpCall(ExFreePoolWithTag, Ptr, 'KgxD');
}

//basic utils
PVOID FindSection(PVOID ModBase, const char* Name, PULONG SectSize)
{
	//get & enum sections
	PIMAGE_NT_HEADERS NT_Header = NT_HEADER(ModBase);
	PIMAGE_SECTION_HEADER Sect = IMAGE_FIRST_SECTION(NT_Header);
	for (PIMAGE_SECTION_HEADER pSect = Sect; pSect < Sect + NT_Header->FileHeader.NumberOfSections; pSect++)
	{
		//copy section name
		char SectName[9]; SectName[8] = 0;
		*(ULONG64*)&SectName[0] = *(ULONG64*)&pSect->Name[0];

		//check name
		if (StrICmp(Name, SectName, true))
		{
			//save size
			if (SectSize) {
				ULONG SSize = SizeAlign(max(pSect->Misc.VirtualSize, pSect->SizeOfRawData));
				*SectSize = SSize;
			}

			//ret full sect ptr
			return (PVOID)((ULONG64)ModBase + pSect->VirtualAddress);
		}
	}

	//no section
	return nullptr;
}

PUCHAR FindPatternSect(PVOID ModBase, const char* SectName, const char* Pattern)
{
	//find pattern utils
	#define InRange(x, a, b) (x >= a && x <= b) 
	#define GetBits(x) (InRange(x, '0', '9') ? (x - '0') : ((x - 'A') + 0xA))
	#define GetByte(x) ((UCHAR)(GetBits(x[0]) << 4 | GetBits(x[1])))

	//get sect range
	ULONG SectSize;
	PUCHAR ModuleStart = (PUCHAR)FindSection(ModBase, SectName, &SectSize);
	PUCHAR ModuleEnd = ModuleStart + SectSize;

	//scan pattern main
	PUCHAR FirstMatch = nullptr;
	const char* CurPatt = Pattern;
	for (; ModuleStart < ModuleEnd; ++ModuleStart)
	{
		bool SkipByte = (*CurPatt == '\?');
		if (SkipByte || *ModuleStart == GetByte(CurPatt)) {
			if (!FirstMatch) FirstMatch = ModuleStart;
			SkipByte ? CurPatt += 2 : CurPatt += 3;
			if (CurPatt[-1] == 0) return FirstMatch;
		}

		else if (FirstMatch) {
			ModuleStart = FirstMatch;
			FirstMatch = nullptr;
			CurPatt = Pattern;
		}
	}

	//failed
	return nullptr;
}

PVOID NQSI(SYSTEM_INFORMATION_CLASS Class)
{
	//get alloc size
	NewTry: ULONG ReqSize = 0;
	ImpCall(ZwQuerySystemInformation, Class, nullptr, ReqSize, &ReqSize);
	if (!ReqSize) goto NewTry;

	//call QuerySystemInfo
	PVOID pInfo = KAlloc(ReqSize);
	if (!NT_SUCCESS(ImpCall(ZwQuerySystemInformation, Class, pInfo, ReqSize, &ReqSize))) {
		KFree(pInfo); goto NewTry;
	}

	//ret buff
	return pInfo;
}

PVOID GetProcAdress(PVOID ModBase, const char* Name)
{
	//parse headers
	PIMAGE_NT_HEADERS NT_Head = NT_HEADER(ModBase);
	PIMAGE_EXPORT_DIRECTORY ExportDir = (PIMAGE_EXPORT_DIRECTORY)((ULONG64)ModBase + NT_Head->OptionalHeader.DataDirectory[0].VirtualAddress);

	//process records
	for (ULONG i = 0; i < ExportDir->NumberOfNames; i++)
	{
		//get ordinal & name
		USHORT Ordinal = ((USHORT*)((ULONG64)ModBase + ExportDir->AddressOfNameOrdinals))[i];
		const char* ExpName = (const char*)ModBase + ((ULONG*)((ULONG64)ModBase + ExportDir->AddressOfNames))[i];

		//check export name
		if (StrICmp(Name, ExpName, true))
			return (PVOID)((ULONG64)ModBase + ((ULONG*)((ULONG64)ModBase + ExportDir->AddressOfFunctions))[Ordinal]);
	}

	//no export
	return nullptr;
}

_FI void Sleep(LONG64 MSec) {
	LARGE_INTEGER Delay; Delay.QuadPart = -MSec * 10000;
	ImpCall(KeDelayExecutionThread, KernelMode, false, &Delay);
}

//process utils
_FI PEPROCESS AttachToProcess(HANDLE PID)
{
	//get eprocess
	PEPROCESS Process = nullptr;
	if (ImpCall(PsLookupProcessByProcessId, PID, &Process) || !Process)
		return nullptr;

	//take process lock
	if (ImpCall(PsAcquireProcessExitSynchronization, Process))
	{
		//process lock failed
		ImpCall(ObfDereferenceObject, Process);
		return nullptr;
	}

	//attach to process
	ImpCall(KeAttachProcess, Process);
	return Process;
}

_FI void DetachFromProcess(PEPROCESS Process)
{
	//check valid process
	if (Process != nullptr)
	{
		//de-attach to process
		ImpCall(KeDetachProcess);

		//cleanup & process unlock
		ImpCall(ObfDereferenceObject, Process);
		ImpCall(PsReleaseProcessExitSynchronization, Process);
	}
}

PEPROCESS GetProcess(const char* ProcName)
{
	//get process list
	PEPROCESS EProc = nullptr;
	PSYSTEM_PROCESS_INFO pInfo = (PSYSTEM_PROCESS_INFO)NQSI(SystemProcessInformation), pInfoCur = pInfo;

	while (true)
	{
		//get process name
		const wchar_t* ProcessName = pInfoCur->ImageName.Buffer;
		if (ImpCall(MmIsAddressValid,(PVOID)ProcessName))
		{
			//check process name
			if (StrICmp(ProcName, ProcessName, true))
			{
				//attach to process
				PEPROCESS Process = AttachToProcess(pInfoCur->UniqueProcessId);
				if (Process != nullptr)
				{
					EProc = Process;
					break;

					//failed
					DetachFromProcess(Process);
				}
			}
		}

		//goto next process entry
		if (!pInfoCur->NextEntryOffset) break;
		pInfoCur = (PSYSTEM_PROCESS_INFO)((ULONG64)pInfoCur + pInfoCur->NextEntryOffset);
	}

	//cleanup
	KFree(pInfo);
	return EProc;
}

PVOID GetUserModuleBase(PEPROCESS Process, const char* ModName)
{
	if (ImpCall(IoIs32bitProcess, nullptr)) {
		PPEB32 pPeb32 = (PPEB32)ImpCall(PsGetProcessWow64Process, Process);
		if (!pPeb32 || !pPeb32->Ldr) return nullptr;

		for (PLIST_ENTRY32 pListEntry = (PLIST_ENTRY32)((PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList.Flink;
			pListEntry != &((PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList;
			pListEntry = (PLIST_ENTRY32)pListEntry->Flink) {
			PLDR_DATA_TABLE_ENTRY32 pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);
			if (StrICmp(ModName, (PWCH)pEntry->BaseDllName.Buffer, false))
				return (PVOID)pEntry->DllBase;
		}
	}
	else {
		PPEB PEB = ImpCall(PsGetProcessPeb, Process);
		if (!PEB || !PEB->Ldr) return nullptr;

		for (PLIST_ENTRY pListEntry = PEB->Ldr->InLoadOrderModuleList.Flink;
			pListEntry != &PEB->Ldr->InLoadOrderModuleList;
			pListEntry = pListEntry->Flink)
		{
			PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

			if (StrICmp(ModName, pEntry->BaseDllName.Buffer, false))
				return pEntry->DllBase;
		}
	}

	return nullptr;
}

_FI PVOID UAlloc(ULONG Size, ULONG Protect = PAGE_READWRITE) {
	PVOID AllocBase = nullptr; SIZE_T SizeUL = SizeAlign(Size);
	if (!ImpCall(ZwAllocateVirtualMemory, ZwCurrentProcess(), &AllocBase, 0, &SizeUL, MEM_COMMIT, Protect)) {
		MemZero(AllocBase, SizeUL);
	}
	return AllocBase;
}

_FI void UFree(PVOID Ptr) {
	SIZE_T SizeUL = 0;
	ImpCall(ZwFreeVirtualMemory, ZwCurrentProcess(), &Ptr, &SizeUL, MEM_RELEASE);
}

//kernel utils
PEPROCESS GetProcessWModule(const char* ProcName, const char* ModName, PVOID* WaitModBase)
{
	//get process list
	PEPROCESS EProc = nullptr;
	PSYSTEM_PROCESS_INFO pInfo = (PSYSTEM_PROCESS_INFO)NQSI(SystemProcessInformation), pInfoCur = pInfo;

	while (true)
	{
		//get process name
		const wchar_t* ProcessName = pInfoCur->ImageName.Buffer;
		if (ImpCall(MmIsAddressValid, (PVOID)ProcessName))
		{
			//check process name
			if (StrICmp(ProcName, ProcessName, true))
			{
				//attach to process
				PEPROCESS Process = AttachToProcess(pInfoCur->UniqueProcessId);
				if (Process != nullptr)
				{
					//check wait module
					PVOID ModBase = GetUserModuleBase(Process, ModName);
					if (ModBase)
					{
						//save modbase
						if (WaitModBase)
							*WaitModBase = ModBase;

						//save eprocess
						EProc = Process;
						break;
					}

					//failed, no wait module
					DetachFromProcess(Process);
				}
			}
		}

		//goto next process entry
		if (!pInfoCur->NextEntryOffset) break;
		pInfoCur = (PSYSTEM_PROCESS_INFO)((ULONG64)pInfoCur + pInfoCur->NextEntryOffset);
	}

	//cleanup
	KFree(pInfo);
	return EProc;
}

PVOID GetKernelModuleBase(const char* ModName)
{
	//get module list
	PSYSTEM_MODULE_INFORMATION ModuleList = (PSYSTEM_MODULE_INFORMATION)NQSI(SystemModuleInformation);

	//process module list
	PVOID ModuleBase = 0;
	for (ULONG64 i = 0; i < ModuleList->ulModuleCount; i++)
	{
		SYSTEM_MODULE Module = ModuleList->Modules[i];
		if (StrICmp(&Module.ImageName[Module.ModuleNameOffset], ModName, true)) {
			ModuleBase = Module.Base;
			break;
		}
	}

	//cleanup
	KFree(ModuleList);
	return ModuleBase;
}


constexpr std::uintptr_t MinimumUserAddress = 0x0000000000010000;
constexpr std::uintptr_t MaximumUserAddress = 0x00007FFFFFFFFFFF;

static bool IsValidPtr(uint64_t address)
{
	return (address && sizeof(address)) ? true : false;
}

inline static std::uintptr_t ToAddress(const void* pointer)
{
	return reinterpret_cast<std::uintptr_t>(pointer);
}

inline static bool IsUserAddress(std::uintptr_t address)
{
	return (address >= MinimumUserAddress && address <= MaximumUserAddress);
}

inline static bool IsUserAddress(const void* pointer)
{
	const auto address = ToAddress(pointer);
	return IsUserAddress(address);
}

inline static bool IsAddressValid(std::uintptr_t address)
{
	return IsUserAddress(address);
}
#include <memory>

template <typename T, typename A>
__forceinline T Read(A Address)
{
	if (ImpCall(MmIsAddressValid, (PVOID)Address))
	{
		return *(T*)Address;
	}

	return T{};
}

__forceinline bool __fastcall ReadArray(uintptr_t Addr, uintptr_t Buff, uintptr_t Size)
{
	if (IsAddressValid(Addr))
	{
		MemCpy2(Buff, Addr, Size);
		return true;
	}

	return false;
}
__forceinline const wchar_t* __fastcall read_wstring(uintptr_t String_address)
{
	wchar_t buffer[512 * 2];
	ReadArray(String_address, (uintptr_t)buffer, 512 * 2);
	return (const wchar_t*)(buffer);
}


template<typename WriteType, typename A>
__forceinline void Write(A Addr, WriteType Data)
{
	if (ImpCall(MmIsAddressValid, (PVOID)Addr)) {
		*(WriteType*)Addr = Data;
	}
}

template <typename T>
__forceinline T ReadChain(std::initializer_list<uintptr_t> Offsets, bool ReadFirstOffset = false)
{
	auto Start = Offsets.begin();
	size_t ReadSize = Offsets.size();
	uintptr_t LastPtr = Read<uintptr_t>((ReadFirstOffset ? Read<uintptr_t>(Start[0]) : Start[0]) + Start[1]);
	for (size_t i = 2; i < ReadSize - 1; i++)
		if (!LastPtr || !(LastPtr = Read<uintptr_t>(LastPtr + Start[i])))
			return T{};

	return Read<T>(LastPtr + Start[ReadSize - 1]);
}

#define safe_read(Addr, Type) Read<Type>(Addr)
#define safe_write(Addr, Data, Type) Write<Type>(Addr, Data)

namespace Win32K {
	ULONG64 gafAsyncKeyState;
	ULONG64 gafAsyncKeyStateRecentDown;
}
unsigned __int16 GetAsyncKeyState(unsigned int a1)
{
	__int16 result;

	if (a1 >= 0x100) { result = 0; }
	else
	{
		unsigned __int64 v1 = (unsigned __int8)a1;
		unsigned __int64 v2 = (unsigned __int64)(unsigned __int8)a1 >> 3;

		__int16 v4 = 0;
		unsigned int v3 = v1 & 7;

		int v5 = *(unsigned __int8*)(v2 + EPtr(Win32K::gafAsyncKeyStateRecentDown));

		if (_bittest((LONG*)&v5, v3)) {
			v4 = 1;
			*(UCHAR*)(v2 + EPtr(Win32K::gafAsyncKeyStateRecentDown)) = v5 & ~(1 << v3);
		}

		result = v4 | 0x8000;
		if (!(*((UCHAR*)EPtr(Win32K::gafAsyncKeyState) + (v1 >> 2)) & (unsigned __int8)(1 << (2 * (v1 & 3)))))
			result = v4;
	}

	return result;
}


