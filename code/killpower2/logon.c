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
	DWORD dwSize;
	HANDLE hToken;
	LPVOID lpvEnv;
	PROCESS_INFORMATION pi;
	PROFILEINFOW profile;
	STARTUPINFOW si;
	WCHAR szUserProfile[256];
	WCHAR szUser[128];
	WCHAR szPass[128];
	WCHAR szDomain[128];
	WCHAR szApp[128];
	WCHAR szPath[256];

	ZeroMemory(&profile, sizeof(profile));
	ZeroMemory(szUserProfile, sizeof(szUserProfile)/sizeof(WCHAR));
	ZeroMemory(szUser, sizeof(szUser)/sizeof(WCHAR));
	ZeroMemory(szPass, sizeof(szPass)/sizeof(WCHAR));
	ZeroMemory(szDomain, sizeof(szDomain)/sizeof(WCHAR));
	ZeroMemory(szApp, sizeof(szApp)/sizeof(WCHAR));
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	if(!MByteToUnicode(username, szUser, sizeof(szUser)/sizeof(WCHAR)))
		return FALSE;
	if(!MByteToUnicode(password, szPass, sizeof(szPass)/sizeof(WCHAR)))
		return FALSE;
	if(!MByteToUnicode(domain, szDomain, sizeof(szDomain)/sizeof(WCHAR)))
		return FALSE;
	if(!MByteToUnicode(app, szApp, sizeof(szApp)/sizeof(WCHAR)))
		return FALSE;

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

	/* Load User Profile */
	profile.dwSize = sizeof(profile);
	profile.lpUserName = szUser;
	if(!LoadUserProfileW(hToken, &profile)) {
		fprintf(stderr, "Error: LoadUserProfile() function failed.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	/* Get User Profile Directory */
	dwSize = sizeof(szUserProfile)/sizeof(WCHAR);
	if(!GetUserProfileDirectoryW(hToken, szUserProfile, &dwSize)) {
		fprintf(stderr, "Error: GetUserProfileDirectory() function failed.\nError code %lu\n",
			GetLastError());
		return FALSE;
	}

	ZeroMemory(szPath, sizeof(szPath)/sizeof(WCHAR));
	GetSystemDirectoryW(szPath, sizeof(szPath)/sizeof(WCHAR));

	si.cb = sizeof(STARTUPINFOW);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	if(!CreateProcessWithLogonW(szUser, szDomain, szPass,
			LOGON_WITH_PROFILE, szApp, NULL, CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,
			lpvEnv, szPath, &si, &pi)) {
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
