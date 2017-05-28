/* Helper functions for create a process as a user.
 * by Philip Simonson.
 */

#include <windows.h>
#include <stdio.h>

BOOL CreateRemoteProcess(LPCSTR username, LPCSTR domain, LPCSTR password,
		DWORD logonType, DWORD logonProvider)
{
	HANDLE hToken;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFOW);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	if(!LogonUser(username, domain, password, logonType,
			logonProvider, &hToken)) {
		fprintf(stderr, "Error: LogonUser() function failed.\n");
		return FALSE;
	}

	if(!ImpersonateLoggedOnUser(hToken)) {
		fprintf(stderr, "Error: Cannot impersonate user account.");
		return FALSE;
	}

	if(!CreateProcessAsUser(hToken, "C:\\Windows\\System32\\Notepad.exe",
			NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL,
			&si, &pi)) {
		fprintf(stderr, "Error: Cannot create the process.\n");
		return FALSE;
	}

	CloseHandle(hToken);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return TRUE;
}
