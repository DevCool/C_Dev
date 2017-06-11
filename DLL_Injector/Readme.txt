=========================================================
Simple DLL Injector By ThePlague1988 (Philip Simonson)
=========================================================

Please don't use this for "hacking" games or anything.
I made this because, I was bored and wanted to have a
go at making a DLL Injector. Use this instead for learning
purposes. I am NOT responsible for your actions, you are.

Will Inject a specified DLL into as many processes as you
input into my custom get_line function.

------------------------------------------------------------
|         dllInjector.dll  -  Function Outline             |
------------------------------------------------------------

extern char* get_line(void);
|
 ------	get_line - returns a pointer to a c string.

extern BOOL InjectDLL(DWORD hProcessID, LPCSTR lpszDLLPath);
|
 ------ InjectDLL - takes process id from GetProcessId() or GetWindowThreadProcessId()
                    and also takes the full path (unless your dll file resides where
		    the program is) of the DLL File.

extern BOOL InjectDLLFromHandle(HMODULE hProcess, LPCSTR lpszDLLPath);
|
 ------ InjectDLLFromHandle - same as InjectDLL but uses process handle instead of
			      PID.

extern HMODULE GetProcessByName(const char *szProcName);
|
 ------ GetProcessByName - Enter the name of the process with the exe or com extension.
	|
	 ----- Example: HMODULE hProcess = GetProcessByName("notepad.exe");

    That should get the first Notepad.exe process found in memory. If no instance of
notepad was found it will fail but, there is error checking.

============================================================
