#include <windows.h>
#include <stdio.h>

#include "dllInjector.h"
#include "objbase.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (__stdcall *MESSAGEBOX)(HWND, LPCWSTR, LPCWSTR, UINT);
typedef HWND (__stdcall *GETWINDOW)(VOID);
typedef BOOL (__stdcall *SETCONSOLEATTR)(HANDLE, WORD);

struct DASHIT {
	MESSAGEBOX MsgBox;
	GETWINDOW GetWindow;
	SETCONSOLEATTR SetConsoleAttr;
	HWND hConsole;
	WORD wAttr;
} textAttr;

int InitTextAttr(struct DASHIT *attr, MESSAGEBOX MsgBox, GETWINDOW GetWindow, SETCONSOLEATTR SetConsoleAttr)
{
	memset(attr, 0, sizeof(struct DASHIT));
	attr->MsgBox = MsgBox;
	attr->GetWindow = GetWindow;
	attr->SetConsoleAttr = SetConsoleAttr;
	return 0;
}

void testfunc(struct DASHIT *attr)
{
	attr->MsgBox(NULL, L"This was a test!", L"It works!", MB_OK | MB_ICONINFORMATION);
}

void PRS_Init(void)
{
	DWORD dwProcessId;
	HMODULE hKernel;
	HWND hConWnd;
	int res;

	printf("DLL: This is the custom PRS_init function!\n");
	dwProcessId = GetCurrentProcessId();

	MESSAGEBOX MsgBox = (MESSAGEBOX)GetProcAddress(GetModuleHandle("user32.dll"), "MessageBoxW");
	if (MsgBox) {
		MsgBox(NULL, L"This is a test message box!", L"Info", MB_OK | MB_ICONINFORMATION);
	}

	hKernel = LoadLibrary("Kernel32.dll");
	if(!hKernel) {
		char message[512];
		WCHAR wszMessage[512];
		memset(wszMessage, 0, sizeof(wszMessage)/sizeof(WCHAR));
		sprintf(message, "Error: Couldn't load kernel32.dll!\nError Code: %d", GetLastError());
		memcpy(wszMessage, message, sizeof(wszMessage)/sizeof(WCHAR));
		MsgBox(NULL, wszMessage, L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	SETCONSOLEATTR SetConsoleAttr = (SETCONSOLEATTR)GetProcAddress(hKernel, "SetConsoleTextAttrW");
	GETWINDOW GetConsole = (GETWINDOW)GetProcAddress(hKernel, "GetConsoleWindowW");
	if (GetConsole) {
		hConWnd = GetConsole();
	}
	if (!hConWnd) {
		MsgBox(NULL, L"Cannot get console window.", L"Warning!", MB_OK | MB_ICONINFORMATION);
	}

	/* Do some shit! */
	res = InitTextAttr(&textAttr, MsgBox, GetConsole, SetConsoleAttr);
	if(res) {
		MsgBox(NULL, L"Error occured setting up textAttr!", L"Error", MB_OK | MB_ICONERROR);
		FreeLibrary(hKernel);
		return;
	}

	/* test function */
	testfunc(&textAttr);
	
	FreeLibrary(hKernel);
}

object InitFuncProto = {
	.InitFunc = PRS_Init
};

DWORD WINAPI DoStuff(LPVOID lpParam)
{
	initFunc = NEW(InitFunc);
	if(initFunc == NULL)
		return E_FAIL;
	initFunc->_(InitFunc)();

	return S_OK;
}

__declspec(dllexport) BOOL APIENTRY DllMain(HMODULE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {
		case DLL_PROCESS_ATTACH:
			DWORD dwThreadId;
			DisableThreadLibraryCalls(hDLL);
			OutputDebugString("DLL: Process_Attach()\n");
			CreateThread(NULL, 0, DoStuff, NULL, 0, &dwThreadId);
		break;

		case DLL_PROCESS_DETACH:
			if(initFunc != NULL) initFunc->_(Destroy)(initFunc);
			OutputDebugString("DLL: Process_Detach()\n");
			Sleep(2500);
		break;
	}

	return TRUE;
}

#ifdef __cplusplus
}
#endif