#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#pragma comment (lib, "urlmon.lib")
#include <urlmon.h>
#include <filesystem>
 #include "kdmapper/kdmapper.hpp"


std::vector<uint8_t> raw_image;


HANDLE iqvw64e_device_handle;

LONG WINAPI SimplestCrashHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
	if (ExceptionInfo && ExceptionInfo->ExceptionRecord)
		Log(_xor_(L"[!!] Crash at addr 0x").c_str() << ExceptionInfo->ExceptionRecord->ExceptionAddress << _xor_(L" by 0x").c_str() << std::hex << ExceptionInfo->ExceptionRecord->ExceptionCode << std::endl);
	else
		Log(_xor_(L"[!!] Crash").c_str() << std::endl);

	if (iqvw64e_device_handle)
		intel_driver::Unload(iqvw64e_device_handle);

	return EXCEPTION_EXECUTE_HANDLER;
}
bool callbackExample(ULONG64* param1, ULONG64* param2, ULONG64 allocationPtr, ULONG64 allocationSize, ULONG64 mdlptr) {
	UNREFERENCED_PARAMETER(param1);
	UNREFERENCED_PARAMETER(param2);
	UNREFERENCED_PARAMETER(allocationPtr);
	UNREFERENCED_PARAMETER(allocationSize);
	UNREFERENCED_PARAMETER(mdlptr);
	Log(_xor_("[+] Callback example called").c_str() << std::endl);

	return true;
}
void BypassLoader() {
	iqvw64e_device_handle = intel_driver::Load();

	NTSTATUS exitCode = 0;
	if (!kdmapper::MapDriver(iqvw64e_device_handle, raw_image.data(), 0, 0, false, true, true, false, callbackExample, &exitCode)) {
		Log(_xor_(L"[-] Failed to map ").c_str() << _xor_("shxultragoodselfdriver").c_str() << std::endl);
		intel_driver::Unload(iqvw64e_device_handle);
	}

	if (!intel_driver::Unload(iqvw64e_device_handle)) {
		Log(_xor_(L"[-] Warning failed to fully unload vulnerable driver ").c_str() << std::endl);
	}	
	Log(_xor_(L"[+] success").c_str() << std::endl);
}