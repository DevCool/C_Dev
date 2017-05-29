/* Helper functions for create a process as a user.
 * by Philip Simonson.
 */

#include <windows.h>
#include <userenv.h>
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

BOOL CreateRemoteProcess(WCHAR *username, WCHAR *domain, WCHAR *password, WCHAR *app)
{
	DWORD dwSize;
	HANDLE hToken;
	LPVOID lpvEnv;
	PROCESS_INFORMATION pi;
	STARTUPINFOW si;
	WCHAR szUserProfile[256];

	ZeroMemory(szUserProfile, sizeof(szUserProfile)/sizeof(WCHAR));
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	if(!LogonUserW(username, domain, password, LOGON32_LOGON_INTERACTIVE,
			LOGON32_PROVIDER_DEFAULT, &hToken)) {
		fprintf(stderr, "Error: LogonUser() function failed.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	if(!CreateEnvironmentBlock(&lpvEnv, hToken, TRUE)) {
		fprintf(stderr, "Error: CreateEnvBlock() function failed.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	dwSize = sizeof(szUserProfile)/sizeof(WCHAR);
	if(!GetUserProfileDirectoryW(hToken, szUserProfile, &dwSize)) {
		fprintf(stderr, "Error: GetUserProfileDirectory() function failed.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	si.cb = sizeof(STARTUPINFOW);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	if(!CreateProcessWithLogonW(username, domain, password, LOGON_WITH_PROFILE, app,
			NULL, CREATE_NEW_CONSOLE, lpvEnv, szUserProfile, &si, &pi)) {
		fprintf(stderr, "Error: Cannot create the process.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	/* Destroy environment block */
	if(!DestroyEnvironmentBlock(lpvEnv)) {
		fprintf(stderr, "Error: Cannot destroy environment block.\nError code %lu\n",
			GetLastError());
		CloseHandle(hToken);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
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
