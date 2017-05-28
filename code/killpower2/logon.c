/* Helper functions for create a process as a user.
 * by Philip Simonson.
 */

#include <windows.h>
#include <stdio.h>

BOOL CreateRemoteProcess(LPCSTR username, LPCSTR domain, LPCSTR password,
		DWORD logonType, DWORD logonProvider)
{
	HANDLE hToken;
	HANDLE hPrimaryToken;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFOW);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	if(!LogonUser(username, domain, password, logonType,
			logonProvider, &hToken)) {
		fprintf(stderr, "Error: LogonUser() function failed.\nError code: %lu\n",
			GetLastError());
		return FALSE;
	}

	if(!DuplicateTokenEx(hToken, TOKEN_QUERY | TOKEN_IMPERSONATE,
			NULL, SecurityImpersonation, TokenPrimary,
			&hPrimaryToken)) {
		fprintf(stderr, "Error: Cannot duplicate token.\nError code: %lu\n",
			GetLastError());
		return FALSE;
	}

	if(!ImpersonateLoggedOnUser(hToken)) {
		fprintf(stderr, "Error: Cannot impersonate user account.\nError code: %lu\n",
			GetLastError());
		return FALSE;
	}

	if(!CreateProcessAsUser(hPrimaryToken, NULL,
			"C:\\Windows\\System32\\cmd.exe", NULL, NULL, FALSE, DETACHED_PROCESS,
			NULL, NULL, &si, &pi)) {
		fprintf(stderr, "Error: Cannot create the process.\nError code: %lu\n",
			GetLastError());
		return FALSE;
	}

	CloseHandle(hToken);
	CloseHandle(hPrimaryToken);
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
