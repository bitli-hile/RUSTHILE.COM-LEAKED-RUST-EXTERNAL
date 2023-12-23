#include "Global.h"
#include <cmath>
#include <cfloat>
#include <array>



#define BaseNetworkable_c 53726152 //  "Signature": "BaseNetworkable_c*"
#define OclusionCulling 54252080 //  "Signature": "OclusionCulling_c*"
#define ConvarCullingC 53830728 //  "Signature": "ConvarCulling_c*"
#define ConsoleSystem_Index_c 53823840 // "Signature": "ConsoleSystem_Index_c*"
#define ConVar_Admin_c 53683056 // "Signature": "ConVar_Admin_c*"

namespace M
{
    uintptr_t GameAssembly;
}

extern "C" __declspec(dllimport) PLIST_ENTRY NTAPI PsLoadedModuleList;



//tmp hook data
PVOID* xKdEnumerateDebuggingDevicesPtr;
PVOID xKdEnumerateDebuggingDevicesVal;

#pragma code_seg(push)
#pragma code_seg("INIT")

//create thread meme
bool SetupKernelThread(PVOID KBase, PVOID ThreadStartAddr)
{
    //get thread fake start address
    PVOID hMsVCRT = nullptr;
    auto Process = GetProcessWModule(E("explorer.exe"), E("msvcrt"), &hMsVCRT);
    auto FakeStartAddr = (PUCHAR)GetProcAdress(hMsVCRT, E("_endthreadex")) + 0x30;

    auto win32k = GetKernelModuleBase(E("win32kbase.sys"));
    Win32K::gafAsyncKeyState = (ULONG64)EPtr(GetProcAdress(win32k, E("gafAsyncKeyState")));
    Win32K::gafAsyncKeyStateRecentDown = (ULONG64)EPtr(GetProcAdress(win32k, E("gafAsyncKeyStateRecentDown")));

    //get usermode func
    auto Var = UAlloc(0x1000); HANDLE Thread = nullptr;
    auto hNtdll = GetUserModuleBase(Process, E("ntdll"));
    auto CallBack = GetProcAdress(hNtdll, E("NtQueryAuxiliaryCounterFrequency"));

    //set kernel hook
    xKdEnumerateDebuggingDevicesPtr = (PVOID*)RVA((ULONG64)EPtr(KeQueryAuxiliaryCounterFrequencyFn) + 4, 7);
    xKdEnumerateDebuggingDevicesVal = _InterlockedExchangePointer(xKdEnumerateDebuggingDevicesPtr, ThreadStartAddr);

    //create new thread
    CLIENT_ID Cid;
    ImpCall(RtlCreateUserThread, ZwCurrentProcess(), nullptr, false, 0, 0, 0, CallBack, Var, &Thread, &Cid);

    if (Thread)
    {
        //close useless handle
        ImpCall(ZwClose, Thread);

        //spoof thread start address
        PETHREAD EThread;
        ImpCall(PsLookupThreadByThreadId, Cid.UniqueThread, &EThread);
        auto StartAddrOff = *(USHORT*)(FindPatternSect(KBase, E("PAGE"), E("48 89 86 ? ? ? ? 48 8B 8C")) + 3);
        *(PVOID*)((ULONG64)EThread + StartAddrOff/*Win32StartAddress*/) = FakeStartAddr;
        ImpCall(ObfDereferenceObject, EThread);

        //wait exec kernel callback
        while (xKdEnumerateDebuggingDevicesPtr && xKdEnumerateDebuggingDevicesVal)
        {
            //Sleep(1);
        }
    }

    //cle   anup
    UFree(Var);
    DetachFromProcess(Process);

    //ret create status
    return (bool)Thread;
}


#define oItemDefinition 0x18 // public ItemDefinition info;
#define oItemid 0x18 // public int itemid;
#define oHeld 0xB0  // private EntityRef heldEntity
#define oActiveUID 0x7D0 // private ItemId clActiveItem;
#define oPlayerInventory 0x8C0 // public PlayerInventory inventory;
#define oContainerBelt 0x28 // public ItemContainer containerBelt;
#define oBeltContentList 0x40 // public List<Item> contents   or   public List<int> availableSlots;
#define oRecoilProperties 0x2E0 // public RecoilProperties recoil;
#define oRecoilYawMin 0x18  // public float recoilYawMin;
#define oRecoilYawMax 0x1C  // public float recoilYawMax;
#define oRecoilPitchMin 0x20 // public float recoilPitchMin;
#define oRecoilPitchMax 0x24 // public float recoilPitchMax;

class WeaponData
{
public:
    int __fastcall GetItemID()
    {
        auto Info = Read<uintptr_t>((const uintptr_t)this + oItemDefinition);
        return Read<int>((const uintptr_t)Info + oItemid);
    }
    int __fastcall GetUID()
    {
        return Read<int>((const uintptr_t)this + 0x20);
    }
};

class BasePlayer
{
public:
    WeaponData* GetWeaponInfo(int Id)
    {
        const uintptr_t Inventory = safe_read(reinterpret_cast<uintptr_t>(this) + oPlayerInventory, uintptr_t);
        const uintptr_t Belt = safe_read(Inventory + 0x28, uintptr_t);
        const uintptr_t ItemList = safe_read(Belt + 0x40, uintptr_t); // public List<Item> itemList; // 0x40
        const uintptr_t Items = safe_read(ItemList + 0x10, uintptr_t);
        WeaponData* WeaponInfo = (WeaponData*)safe_read(Items + 0x20 + (Id * 0x8), uintptr_t);
        return WeaponInfo;
    }
    WeaponData* GetActiveWeapon()
    {
        const int ActUID = safe_read(reinterpret_cast<uintptr_t>(this) + oActiveUID, int);
        if (!ActUID) return nullptr;
        WeaponData* ActiveWeapon;
        for (int i = 0; i < 6; i++) if (ActUID == (ActiveWeapon = GetWeaponInfo(i))->GetUID()) return ActiveWeapon;
        return nullptr;
    }
};

class LPlayerBase
{
public:
    BasePlayer* BasePlayer = nullptr;
};
LPlayerBase LocalPlayer;

bool is_weapon()
{
    uint16_t held_item = LocalPlayer.BasePlayer->GetActiveWeapon()->GetItemID();
    if (held_item == 0x0)    return false;
    if (held_item == 0xB98E)	return true;
    if (held_item == 0xEC45)	return true;
    if (held_item == 0x9A57)	return true;
    if (held_item == 0x13BB)	return true;
    if (held_item == 0x82C3)	return true;
    if (held_item == 0x0EC1)	return true;
    if (held_item == 0x5371)	return true;
    if (held_item == 0xE257)	return true;
    if (held_item == 0xBB78)	return true;
    if (held_item == 0x8D5F)	return true;
    if (held_item == 0x5C8B)	return true;
    if (held_item == 0x31E1)	return true;
    if (held_item == 0x9C37)	return true;
    if (held_item == 0x31E1)	return true;
    if (held_item == 0x2593)	return true;
    if (held_item == 0x142C)	return true;
    if (held_item == 0xE126)	return true;
    if (held_item == 0xEBB5)	return true;
    if (held_item == 0x2D2B)	return true;
    if (held_item == 0x3261)	return true;
    if (held_item == 0x110A)	return true;
    if (held_item == 0x6650)	return true;
    if (held_item == 0xAB32)	return true;
    if (held_item == 0x397F) return true;
    if (held_item == 0xD2D5) return true;
    if (held_item == 0xDEDF) return true;
    else return false;
}

uintptr_t GetHeldItem(uintptr_t LocalPlayer)
{
    uintptr_t base_player = LocalPlayer;
    uintptr_t active_item_id = safe_read(base_player + oActiveUID, uintptr_t);
    uintptr_t player_inventory = safe_read(base_player + oPlayerInventory, uintptr_t);
    uintptr_t inventory_belt = safe_read(player_inventory + oContainerBelt, uintptr_t);
    uintptr_t belt_contents_list = safe_read(inventory_belt + oBeltContentList, uintptr_t);
    uintptr_t belt_contents_sz = safe_read(belt_contents_list + 0x14, uintptr_t);
    uintptr_t belt_contents = safe_read(belt_contents_list + 0x10, uintptr_t);
    for (auto i = 0u; i <= 6; i++)
    {
        if (belt_contents != 0)
        {
            uintptr_t current_item = safe_read(belt_contents + (0x20 + (i * 8)), uintptr_t);
            if (!current_item) continue;
            if (active_item_id == safe_read(current_item + 0x20, uintptr_t)) return current_item;
        }
    }
}

bool BOXESP;
bool NORECOIL;
bool ADMINFLAG;
bool SPIDERMAN; 
bool ALWAYSDAY;
NTSTATUS FakeThread2() {
    ImpCall(KeEnterGuardedRegion);
r5:
    auto Process = GetProcess(E("RustClient.exe"));
    Sleep(20000);
    if (!Process)
        goto r5;
    M::GameAssembly = (uintptr_t)GetUserModuleBase(Process, E("GameAssembly.dll"));
    while (true) {
        if (GetAsyncKeyState(0xE7)) // 0xF8
        {
            DetachFromProcess(Process);
            break;
        }
        uintptr_t h4 = ReadChain<uintptr_t>({ M::GameAssembly, BaseNetworkable_c, 0xB8, 0x0, 0x10, 0x28 }, false);
        LocalPlayer.BasePlayer = (BasePlayer*)safe_read(safe_read(h4 + 0x18, uintptr_t) + 0x20, uintptr_t);
        uintptr_t Local_Player = (uintptr_t)LocalPlayer.BasePlayer;
        if (GetAsyncKeyState(0xF9) & 1)
            BOXESP = !BOXESP;
        if (GetAsyncKeyState(0xF8) & 1)
            NORECOIL = !NORECOIL;
        if (GetAsyncKeyState(0xF7) & 1)
            ADMINFLAG = !ADMINFLAG;
        if (GetAsyncKeyState(0xF6) & 1)
            SPIDERMAN = !SPIDERMAN;
        if (GetAsyncKeyState(0xE8) & 1)
            ALWAYSDAY = !ALWAYSDAY;

        //ESP
        uintptr_t Puffset1 = Read<uintptr_t>(M::GameAssembly + OclusionCulling);
        uintptr_t Puffset2 = Read<uintptr_t>(Puffset1 + 0xB8);
        uintptr_t Puffset4 = Read<uintptr_t>(Puffset2 + 0x70);
        uintptr_t Puffset5 = Read<uintptr_t>(Puffset4 + 0x1A0);
        uintptr_t ConvarCulling = Read<uintptr_t>(M::GameAssembly + ConvarCullingC);
        uintptr_t ConvarCulling2 = Read<uintptr_t>(ConvarCulling + 0xB8);
        Write<float>(ConvarCulling2 + 0x0, 1000.f);
        Write<int>(Puffset5 + 0x20, 131072);
        Write<int>(Puffset2 + 0x8C, (int)BOXESP);
        //
        if (NORECOIL) {
            uintptr_t current_item = GetHeldItem(Local_Player);
            uintptr_t base_projectile = safe_read(current_item + oHeld, uintptr_t);
            uintptr_t recoil_prop = safe_read(base_projectile + oRecoilProperties, uintptr_t);
            uintptr_t newRecoilOverride = safe_read(recoil_prop + 0x78, uintptr_t);
            if (is_weapon())
            {
                if (newRecoilOverride)
                {
                    safe_write(newRecoilOverride + oRecoilYawMin, 0, float);
                    safe_write(newRecoilOverride + oRecoilYawMax, 0, float);
                    safe_write(newRecoilOverride + oRecoilPitchMin, 0, float);
                    safe_write(newRecoilOverride + oRecoilPitchMax, 0, float);
                }
                if (recoil_prop)
                {
                    safe_write(recoil_prop + oRecoilYawMin, 0, float);
                    safe_write(recoil_prop + oRecoilYawMax, 0, float);
                    safe_write(recoil_prop + oRecoilPitchMin, 0, float);
                    safe_write(recoil_prop + oRecoilPitchMax, 0, float);
                }
            }
        }
        if (SPIDERMAN)
        {
            uintptr_t base_movement = Read<uintptr_t>((uintptr_t)LocalPlayer.BasePlayer + 0x6A0);
            safe_write(base_movement + 0xC8, 0.f, float);
        }
        if (ADMINFLAG)
        {
            const uintptr_t ConsoleSystemCommands = safe_read(safe_read(safe_read(M::GameAssembly + ConsoleSystem_Index_c, uintptr_t) + 0xb8, uintptr_t) + 0x10, uintptr_t);
            if (ConsoleSystemCommands)
            {
                const wchar_t command_NoClip[7] = (L"noclip");
                const wchar_t command_DebugCamera[12] = (L"debugcamera");
                for (int i = 0; i < 300; i++)
                {
                    const uintptr_t ConsoleSystemCommand = safe_read(ConsoleSystemCommands + 0x20 + (i * 0x8), uintptr_t);

                    if (ConsoleSystemCommand)
                    {
                        const uintptr_t Name = safe_read(ConsoleSystemCommand + 0x10, uintptr_t); // public string Name; // 0x10	

                        if (Name)
                        {
                            wchar_t command_name[36] = { '\0' };
                            ReadArray(Name + 0x14, (uintptr_t)command_name, sizeof(command_name));

                            if (wcscmp(command_name, command_DebugCamera) == 0 || wcscmp(command_name, command_NoClip) == 0)
                            {
                                safe_write(ConsoleSystemCommand + 0x58, false, bool); // public bool AllowRunFromServer; // 0x58
                            }
                        }
                    }
                }
            }
            auto player_flags = Read<uintptr_t>((uintptr_t)LocalPlayer.BasePlayer + 0x8B0);  //0x740
            player_flags |= 4;
            Write((uintptr_t)LocalPlayer.BasePlayer + 0x8B0, player_flags);
        }
        uintptr_t static_fields2 = ReadChain<uintptr_t>({ M::GameAssembly, ConVar_Admin_c, 0xb8 }, false);
        Write<float>(static_fields2 + 0x0, ALWAYSDAY ? 12 : -1); // public static float admintime;
    }
    ImpCall(KeLeaveGuardedRegion);
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS DriverEntry()
{
    PVOID KBase = *reinterpret_cast<PVOID*>(std::uintptr_t(PsLoadedModuleList) + 0x30);
    ImpSet(IoIs32bitProcess);
    ImpSet(PsGetProcessWow64Process);
    ImpSet(ExAllocatePoolWithTag);
    ImpSet(ExFreePoolWithTag);
    ImpSet(IoGetCurrentProcess);
    ImpSet(KeAttachProcess);
    ImpSet(KeDelayExecutionThread);
    ImpSet(KeDetachProcess);
    ImpSet(KeEnterGuardedRegion);
    ImpSet(KeLeaveGuardedRegion);
    ImpSet(KeQueryAuxiliaryCounterFrequency);
    ImpSet(KeUserModeCallback);
    ImpSet(MmIsAddressValid);
    ImpSet(ObfDereferenceObject);
    ImpSet(PsAcquireProcessExitSynchronization);
    ImpSet(PsGetProcessPeb);
    ImpSet(PsLookupProcessByProcessId);
    ImpSet(PsLookupThreadByThreadId);
    ImpSet(PsReleaseProcessExitSynchronization);
    ImpSet(RtlCreateUserThread);
    ImpSet(ZwAllocateVirtualMemory);
    ImpSet(ZwClose);
    ImpSet(ZwFreeVirtualMemory);
    ImpSet(ZwQuerySystemInformation);
    ImpSet(ZwQueryVirtualMemory);
    ImpSet(MmSecureVirtualMemory);
    ImpSet(ZwProtectVirtualMemory);

    ImpCall(KeEnterGuardedRegion);

    SetupKernelThread(KBase, FakeThread2);

    ImpCall(KeLeaveGuardedRegion);
    return STATUS_SUCCESS;
}

#pragma code_seg(pop)