/* Helper functions for create a process as a user.
 * by Philip Simonson.
 */

#include <windows.h>
#include <userenv.h>
#include <stdio.h>

extern BOOL MByteToUnicode(LPCSTR mbStr, LPWSTR uStr, DWORD dwSize);

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

BOOL CreateRemoteProcess(char *username, const char *domain, const char *password, const char *app)
{
	HANDLE hToken;
	HANDLE hPrimaryToken;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	SECURITY_ATTRIBUTES sa;
	char szPath[256];

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	if(!LogonUser(username, domain, password, LOGON32_LOGON_INTERACTIVE,
			LOGON32_PROVIDER_DEFAULT, &hToken)) {
		fprintf(stderr, "Error: LogonUser() function failed.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	ZeroMemory(&sa, sizeof(sa));
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	if(!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, &sa,
			SecurityIdentification, TokenPrimary, &hPrimaryToken)) {
		fprintf(stderr, "Error: DuplicateToken() function failed.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}
	CloseHandle(hToken);

	if(!ImpersonateLoggedOnUser(hPrimaryToken)) {
		fprintf(stderr, "Error: ImpersonateLoggedOnUser() function failed.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	ZeroMemory(szPath, sizeof(szPath)/sizeof(char));
	GetSystemDirectory(szPath, sizeof(szPath)/sizeof(char));

	si.cb = sizeof(STARTUPINFOW);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	if(!CreateProcessAsUser(hPrimaryToken, app, NULL, 
			&sa, &sa, FALSE, CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,
			NULL, szPath, &si, &pi)) {
		fprintf(stderr, "Error: Cannot create the process.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	CloseHandle(hPrimaryToken);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return TRUE;
}

/*
BOOL CreateRemoteProcess(WCHAR *username, WCHAR *domain, WCHAR *password, WCHAR *app)
{
	DWORD dwSize;
	HANDLE hToken;
	LPVOID lpvEnv;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	WCHAR szUserProfile[256];

	ZeroMemory(szUserProfile, sizeof(szUserProfile)/sizeof(WCHAR));
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	if(!LogonUser(username, domain, password, LOGON32_LOGON_INTERACTIVE,
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
	if(!GetUserProfileDirectory(hToken, szUserProfile, &dwSize)) {
		fprintf(stderr, "Error: GetUserProfileDirectory() function failed.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	if(!CreateProcessWithLogonW(username, domain, password, LOGON_WITH_PROFILE, app,
			NULL, CREATE_NEW_CONSOLE, lpvEnv, szUserProfile, &si, &pi)) {
		fprintf(stderr, "Error: Cannot create the process.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

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
*/

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
