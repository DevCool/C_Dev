#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 2
#define INJECTOR_EXPORTS __declspec(dllexport)
#define PROCESS_THREAD_CREATION (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE)

#ifdef __cplusplus
extern "C" {
#endif

#include "dllInjector.h"

object InitFuncProto = {
	.InitFunc = PRS_Default
};

INJECTOR_EXPORTS char* get_line(void)
{
	char *buffer = NULL;
	int size = BUF_SIZE;
	int pos = 0;
	int c;

	buffer = realloc(buffer, size);

	while(1) {
		c = getchar();
		if(c == EOF || c == '\n') {
			buffer[pos] = 0;
			return buffer;
		}
		else {
			buffer[pos] = c;
		}
		++pos;

		if(pos >= size) {
			size += BUF_SIZE;
			buffer = realloc(buffer, size);
			if (buffer == NULL) {
				printf("Error: Out of memory.\n");
				break;
			}
		}
	}

	return NULL;
}

void __stdcall PRS_Init(void) {
	printf("Dummy function so compiler won't complain.\n");
}

BOOL InjectDLL_ne(DWORD dwPID, LPCSTR lpszDllPath)
{
	LPVOID LoadLibrary, Memory;
	HANDLE hThread, Process;
	DWORD dwExitCode;
	BOOL bThreadSuccess;

	Process = OpenProcess(PROCESS_THREAD_CREATION, FALSE, dwPID);
	if(!Process) {
		return FALSE;
	}

	LoadLibrary = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	if(!LoadLibrary) {
		MessageBox(NULL, "Cannot get the address of LoadLibraryA function.", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	Memory = VirtualAllocEx(Process, NULL, strlen(lpszDllPath)+1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(!Memory) {
		MessageBox(NULL, "Allocation failed...\nCouldn't allocate memory for injection.",
			"Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	WriteProcessMemory(Process, Memory, lpszDllPath, strlen(lpszDllPath)+1, NULL);
	hThread = CreateRemoteThread(Process, NULL, 0, LoadLibrary, Memory, 0, NULL);
	if(hThread) {
		SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
		WaitForSingleObject(hThread, INFINITE);
		if(GetExitCodeThread(hThread, &dwExitCode)) {
			bThreadSuccess = (dwExitCode != 0) ? TRUE : FALSE;
		}
		CloseHandle(hThread);
	}

	VirtualFreeEx(Process, Memory, 0, MEM_RELEASE);
	CloseHandle(Process);

	return TRUE;
}

INJECTOR_EXPORTS BOOL InjectDLL(DWORD dwProcessId, LPCSTR lpszDLLPath)
{
	HANDLE hProcess, hThread;
	LPVOID lpBaseAddr, lpFuncAddr;
	DWORD dwMemSize, dwExitCode;
	BOOL bSuccess = FALSE;
	HMODULE hUserDLL;

	hProcess = OpenProcess(PROCESS_THREAD_CREATION, FALSE, dwProcessId);
	if (!hProcess) {
		MessageBox(NULL, "Sorry process not available.", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	dwMemSize = lstrlen(lpszDLLPath) + 1;
	lpBaseAddr = VirtualAllocEx(hProcess, NULL, dwMemSize, MEM_COMMIT, PAGE_READWRITE);
	if(!lpBaseAddr) {
		MessageBox(NULL, "Cannot proceed with VirtualAlloc!", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if(!WriteProcessMemory(hProcess, lpBaseAddr, lpszDLLPath, dwMemSize, NULL)) {
		MessageBox(NULL, "Writing of process memory failed!", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	hUserDLL = LoadLibrary("kernel32.dll");
	if(!hUserDLL) {
		MessageBox(NULL, "Couldn't load kernel32.dll", "Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if((lpFuncAddr = GetProcAddress(hUserDLL, "LoadLibraryA"))) {
		if((hThread = CreateRemoteThread(hProcess, NULL, 0, lpFuncAddr, lpBaseAddr, 0, NULL))) {
			WaitForSingleObject(hThread, INFINITE);
			if(GetExitCodeThread(hThread, &dwExitCode)) {
				bSuccess = (dwExitCode != 0) ? TRUE : FALSE;
			}
			CloseHandle(hThread);
		}
	}

	FreeLibrary(hUserDLL);
	VirtualFreeEx(hProcess, lpBaseAddr, 0, MEM_RELEASE);
	CloseHandle(hProcess);

	return bSuccess;
}

INJECTOR_EXPORTS BOOL InjectDLLFromHandle(DWORD dwProcessId, LPCSTR lpszDLLPath)
{
	return InjectDLL_ne(dwProcessId, lpszDLLPath);
}

INJECTOR_EXPORTS HANDLE GetProcessByName(const char *szProcName)
{
	DWORD pid = 0;
	/* Create toolhelp snapshot */
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	/* Walkthrough all processes */
	if (Process32First(snapshot, &process)) {
		do {
			/* compare process name to the name given as a parameter
			 * to this function */
			if (stricmp(process.szExeFile, szProcName) == 0) {
				pid = process.th32ProcessID;
				break;
			}
		} while(Process32Next(snapshot, &process));
	}
	CloseHandle(snapshot);

	if (pid != 0)
		return OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);

	// Process not found if it gets here
	return NULL;
}

#ifdef __cplusplus
}
#endif

BOOL WINAPI DllMain(HMODULE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {
		case DLL_PROCESS_ATTACH:
		break;

		case DLL_PROCESS_DETACH:
		break;

		case DLL_THREAD_ATTACH:
		break;

		case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
