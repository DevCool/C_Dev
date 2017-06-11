/*********************************************************************************
* Coded by PRS (5n4k3)
*********************************************************************************
* Compile w/MS: cl tester.c /DEFAULTLIB user32.lib /DEFAULTLIB kernel32.lib
*  /DEFAULTLIB gdi32.lib /O2
*********************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum BOOL {
	FALSE,
	TRUE
};
typedef enum BOOL bool;

#define true 1
#define false 0
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#define BLAH_YEAH "\x50\x52\x53\x20\x53\x4E\x41\x4B\x45\x00"
#define BUFSIZE 1

char *
get_ln(a, s)
bool a;
const char *s;
{
	int iPos = 0;
	int iSize = BUFSIZE;
	char *buffer = NULL;
	int c;

	buffer = realloc(buffer, iSize);
	if (!buffer)
		return (char*)0;

	printf("%s", s);

	while (1) {
		c = getchar();
		if (c == -1 || c == 0x0A) {
			if (iPos >= iSize && a) {
				iSize += BUFSIZE + 2;
				buffer = realloc(buffer, iSize);
				if (!buffer) break;
				*(buffer + iPos) = '\n';
				*(buffer + iPos + 1) = 0x00;
				++iPos;
			}
			else
				*(buffer + iPos) = 0x00;
			return buffer;
		}
		else
			*(buffer + iPos) = c;
		++iPos;

		if (iPos >= iSize) {
			iSize += BUFSIZE;
			buffer = realloc(buffer, iSize);
			if (!buffer) break;
		}
	}

	return (char*)0;
}

struct _tagMAINWND {
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wc;
	HINSTANCE hInst;
	HWND hConsole;
} MainWnd;

int pmain();
int start();
int run();

int (*mainFunc)() = &pmain;

int
main(argc, argv)
int argc;
char **argv;
{
	memset(&MainWnd, 0, sizeof(struct _tagMAINWND));
	MainWnd.hInst = GetModuleHandle(NULL);
	MainWnd.hConsole = GetConsoleWindow();
	return mainFunc(&MainWnd);
}

bool
chg_col(wAttr)
WORD wAttr;
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole) {
		SetConsoleTextAttribute(hConsole, wAttr);
		return TRUE;
	}

	return FALSE;
}

int
pmain(mainWnd)
struct _tagMAINWND *mainWnd;
{
	char *buf = (char*)0;

	chg_col(0x001E);
	printf("%s\n", BLAH_YEAH);
	chg_col(0x000A);
	buf = get_ln(false, "The program is locked... insert passcode.\n>> ");
	if (strcmp(buf, BLAH_YEAH) == 0) {
		printf("Program\a Activated!\a\n");
		if (mainWnd->hConsole) {
			if (start(mainWnd) == 0) {
				chg_col(0x0007);
				if (buf != (char*)0) free(buf);
				return run(mainWnd);
			}
		}
		else {
			MessageBox(NULL, "Could not get the console window.", "Warning",
				MB_OK | MB_ICONEXCLAMATION);
		}
	}
	else {
		printf("You're wrong!\n");
	}

	chg_col(0x0007);
	if (buf != (char*)0) free(buf);
	return 0;
}

#define IDC_STATIC 1000
#define IDC_YODAWG 1001

typedef struct P_RECT {
	int x;
	int y;
	int cx;
	int cy;
} rect;

struct P_THREAD {
	HANDLE hThread;
	bool bThreadAlive;
} timeThread;

DWORD WINAPI
timer_thread(lpParameter)
LPVOID lpParameter;
{
	HWND hWnd = lpParameter;

	return S_OK;
}

LRESULT CALLBACK
main_procedure(hWnd, uMsg, wparam, lparam)
HWND hWnd;
UINT uMsg;
WPARAM wparam;
LPARAM lparam;
{
	HDC hdc;

	switch (uMsg) {

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND: {
		if (LOWORD(wparam) == IDC_YODAWG) {
			MessageBox(NULL, "Hello dawg! :D", "INFO?", MB_OK | MB_ICONINFORMATION);
			SetWindowPos(hWnd, HWND_TOP, 0, 0, 640, 480, SWP_HIDEWINDOW);
			Sleep(2000);
			SetWindowPos(hWnd, HWND_TOP, 10, 10, 640, 480, SWP_SHOWWINDOW);
		}
	} break;
	case WM_CTLCOLORSTATIC: {
		if((HWND)lparam == GetDlgItem(hWnd, IDC_STATIC)) {
			hdc = (HDC)wparam;
			SetBkColor(hdc, (COLORREF)GetSysColor(COLOR_WINDOW));
			SetTextColor(hdc, RGB(255, 0, 255));
			return (INT_PTR)GetSysColorBrush(COLOR_3DFACE);
		}
	} break;
	case WM_CREATE: {
		rect rWin;
		GetClientRect(hWnd, (LPRECT)&rWin);
		HWND hWndBtn = CreateWindow("BUTTON",
			"Click &Me",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			(rWin.cx/2)-(120/2), (rWin.cy/2)-(20/2), 120, 20,
			hWnd, (HMENU)IDC_YODAWG, GetModuleHandle(NULL), NULL);
		if (!hWndBtn) {
			MessageBox(NULL,
				"Cannot create \"Click Me\" button.", "Warning",
				MB_OK | MB_ICONWARNING);
		}
		HWND hWndText = CreateWindow("STATIC",
			"Hello this is some text!",
			WS_VISIBLE | WS_CHILD | SS_CENTER,
			(rWin.cx/2)-(175/2), (rWin.cy/2)-(20/2)-30, 175, 20,
			hWnd, (HMENU)IDC_STATIC, GetModuleHandle(NULL), NULL);
		if (!hWndText) {
			MessageBox(NULL,
				"Cannot create \"Static Text 1\" control.", "Warning",
				MB_OK | MB_ICONWARNING);
		}
		timeThread.hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)timer_thread, hWnd, 0, NULL);
		timeThread.bThreadAlive = true;
	} break;
	default:
		return DefWindowProc(hWnd, uMsg, wparam, lparam);

	}

	return 0;
}

int
start(mainWnd)
struct _tagMAINWND *mainWnd;
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = (WNDPROC)main_procedure;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW+3);
	wc.lpszClassName = (LPCSTR)"MainWindow";
	wc.lpszMenuName = (LPCSTR)NULL;
	wc.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Class registration failed.", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	mainWnd->wc = wc;

	mainWnd->hWnd = CreateWindow("MainWindow", "Simple Window",
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		640, 480, NULL, NULL, mainWnd->hInst, NULL);
	if (!mainWnd->hWnd) {
		MessageBox(NULL, "Couldn't create the window sorry :P", "Error", MB_OK | MB_ICONERROR);
		return -2;
	}

	ShowWindow(mainWnd->hWnd, SW_SHOWNORMAL);
	UpdateWindow(mainWnd->hWnd);

	return 0;
}

int
run(mainWnd)
struct _tagMAINWND *mainWnd;
{
	memset(&mainWnd->msg, 0, sizeof(MSG));

	while (true) {
		if (PeekMessage(&mainWnd->msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&mainWnd->msg);
			DispatchMessage(&mainWnd->msg);
		}

		if (mainWnd->msg.message == WM_QUIT) {
			FreeConsole();
			PostQuitMessage(0);
			break;
		}
		/* add extra code here */
	}

	if (WaitForSingleObject(timeThread.hThread, 1000))
		timeThread.bThreadAlive = FALSE;

	return (int)mainWnd->msg.wParam;
}