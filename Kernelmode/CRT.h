template <typename StrType, typename StrType2>
_FI bool StrICmp(StrType Str, StrType2 InStr, bool Two) 
{
	#define ToLower(Char) ((Char >= 'A' && Char <= 'Z') ? (Char + 32) : Char)

	if (!Str || !InStr) 
		return false;

	wchar_t c1, c2; do {
		c1 = *Str++; c2 = *InStr++;
		c1 = ToLower(c1); c2 = ToLower(c2);
		if (!c1 && (Two ? !c2 : 1))
			return true;
	} while (c1 == c2);

	return false;
}

template <typename StrType> 
_FI int StrLen(StrType Str) {
	if (!Str) return 0;
	StrType Str2 = Str;
	while (*Str2) *Str2++;
	return (int)(Str2 - Str);
}

char* m_strstr(char* input, const char* find)
{
	do {
		const char* p, * q;
		for (p = input, q = find; *q != '\0' && *p == *q; p++, q++) {}
		if (*q == '\0') {
			return input;
		}
	} while (*(input++) != '\0');
	return 0;
}

__forceinline const wchar_t* __fastcall m_strstr2(wchar_t* input, const wchar_t* find)
{
	do {
		const wchar_t* p, * q;
		for (p = input, q = find; *q != '\0' && *p == *q; p++, q++) {}
		if (*q == '\0') {
			return input;
		}
	} while (*(input++) != '\0');
	return 0;
}


_FI void MemCpy(PVOID Dst, PVOID Src, ULONG Size) {
	__movsb((PUCHAR)Dst, (const PUCHAR)Src, Size);
}
_FI void MemCpy2(uintptr_t Dst, uintptr_t Src, uintptr_t Size) {
	__movsb((PUCHAR)Dst, (const PUCHAR)Src, Size);
}
_FI void MemZero(PVOID Ptr, SIZE_T Size, UCHAR Filling = 0) {
	__stosb((PUCHAR)Ptr, Filling, Size);
}