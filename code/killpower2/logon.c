/* Helper functions for create a process as a user.
 * by Philip Simonson.
 */

#include <windows.h>
#include <stdio.h>

BOOL SetTokenPrivilege(HANDLE hToken, LPCSTR szPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if(!LookupPrivilegeValue(NULL, szPrivilege, &luid)) {
		printf("LookupPrivilege failed\n");
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if(bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	/* Enable the privilege or disable all */
	if(!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, 0)) {
		printf("Adjusting token privileges failed.\n");
		return FALSE;
	}

	if(GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
		printf("The token does not have the specified privilege.\n");
		return FALSE;
	}

	return TRUE;
}

BOOL CreateRemoteProcess(LPCSTR username, LPCSTR domain, LPCSTR password)
{
	HANDLE hToken;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_READ | TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES,
			&hToken)) {
		fprintf(stderr, "Error: OpenProcessToken() function failed.\n"
			"Error code %lu\n", GetLastError());
		return FALSE;
	}

	if(!SetTokenPrivilege(hToken, SE_TCB_NAME, TRUE)) {
		printf("TCB_NAME\n");
		return FALSE;
	}
/*
	if(!SetTokenPrivilege(hToken, SE_ASSIGNPRIMARYTOKEN_NAME, TRUE)) {
		printf("ASSIGNPRIMARYTOKEN_NAME\n");
		return FALSE;
	}
	if(!SetTokenPrivilege(hToken, SE_INCREASE_QUOTA_NAME, TRUE)) {
		printf("INCREASE_QUOTA_NAME\n");
		return FALSE;
	}
 */

	if(!LogonUser(username, domain, password, LOGON32_LOGON_INTERACTIVE,
			LOGON32_PROVIDER_DEFAULT, &hToken)) {
		fprintf(stderr, "Error: LogonUser() function failed.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	if(!CreateProcessAsUser(hToken, "C:\\Windows\\System32\\Notepad.exe",
			NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE,
			NULL, NULL, &si, &pi)) {
		fprintf(stderr, "Error: Cannot create the process.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	CloseHandle(hToken);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return TRUE;
}

BOOL LaunchApp(char *appname)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	if(!CreateProcess(
		NULL,
		appname,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		"C:\\Windows\\System32\\",
		&si,
		&pi
	)) {
		fprintf(stderr, "Error code: %lu\n", GetLastError());
		return FALSE;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return TRUE;
}
