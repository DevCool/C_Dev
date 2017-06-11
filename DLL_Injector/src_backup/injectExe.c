#include <stdio.h>
#include <windows.h>

/* Just find the process for now and
 * report the pid back to the user */

int main(int argc, char *argv[])
{
	char *procName = NULL;

	extern BOOL InjectDLLFromHandle(DWORD dwProcessId, LPCSTR lpszDLLPath);
	extern HMODULE GetProcessByName(const char *szProcName);
	extern char* get_line(void);

	while(1) {
		/* Get input from user, then search available
		 * processes in memory for it */
		if (procName != NULL) free(procName);
		procName = get_line();

		if (stricmp(procName, "exit") == 0)
			break;

		if (stricmp(procName, "") != 0) {
			HANDLE hProcess = GetProcessByName(procName);
			if (hProcess) {
				DWORD ProgramPID = GetProcessId(hProcess);
				printf("Process found: [PID : %d]\n",
					ProgramPID);
				if(InjectDLLFromHandle(ProgramPID, "test.dll") == FALSE) {
					char message[256];
					memset(&message, 0, sizeof(message));
					sprintf(message, "Sorry couldn't inject the DLL into the process!\nNAME [%s]\nPID [%d]\nError Code: %d\n",
						procName, ProgramPID, GetLastError());
					MessageBox(NULL, message, "InjectFromHandle : Error!", MB_OK | MB_ICONWARNING);
				}
				else {
					char message[256];
					memset(&message, 0, sizeof(message));
					sprintf(message, "NAME: %s\nPID : %d\nSTAT: Injected\n",
						procName, ProgramPID);
/*					MessageBox(NULL, message, "InjectFromPID : Success!", MB_OK | MB_ICONWARNING); */
					printf("%s", message);
				}
			}
			else {
				char message[256];
				memset(&message, 0, sizeof(message));
				sprintf(message, "[%s] Module not found!\n", procName);
				MessageBox(NULL, message, "Info", MB_OK | MB_ICONINFORMATION);
			}
		}
	}

	if (procName != NULL) free(procName);
	return 0;
}