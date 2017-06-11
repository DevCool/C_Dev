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
#include <time.h>

enum BOOL {
	FALSE,
	TRUE
};
typedef enum BOOL bool;

#define true 1
#define false 0
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#define MAINWND_SIZEX 600
#define MAINWND_SIZEY 480

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
	int res = 0;

	srand(time(NULL));
	chg_col(0x001E);
	printf("%s\n", BLAH_YEAH);
	chg_col(0x000A);
	buf = get_ln(false, "The program is locked... insert passcode.\n>> ");
	if (strcmp(buf, BLAH_YEAH) == 0) {
		printf("Program\a Activated!\a\n");
		if (mainWnd->hConsole) {
			if (start(mainWnd) == 0) {
				chg_col(0x0007);
				SetWindowPos(mainWnd->hConsole, NULL, 0, 0, MAINWND_SIZEX, MAINWND_SIZEY, SWP_HIDEWINDOW);
				res=run(mainWnd);
				SetWindowPos(mainWnd->hConsole, NULL, 0, 0, MAINWND_SIZEX, MAINWND_SIZEY, SWP_SHOWWINDOW);
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
	if(buf!=(char*)0) free(buf);
	return res;
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

static char colR;
static char colG;
static char colB;
static char bActive = 1;
static HDC hdc;
static PAINTSTRUCT ps;
static rect rWin;

#define STATIC_MSG1 "Press F7 to hide a window."
#define STATIC_MSG2 "Press F8 to unhide all windows."
#define STATIC_MSG3 "Press F9 to hide/unhide this app."

void
textout_lines(message, lcnt, rWin)
const char *message;
int lcnt;
rect *rWin;
{
	rect rwin = *rWin;

	if(message == NULL) {
		TextOut(hdc, (rwin.cx/2)-(int)(30*3.25), (rwin.cy/2-120)-(20*0+40), STATIC_MSG1, strlen(STATIC_MSG1));
		TextOut(hdc, (rwin.cx/2)-(int)(30*3.25), (rwin.cy/2-120)-(20*1+40), STATIC_MSG2, strlen(STATIC_MSG2));
		TextOut(hdc, (rwin.cx/2)-(int)(30*3.25), (rwin.cy/2-120)-(20*2+40), STATIC_MSG3, strlen(STATIC_MSG3));
	} else {
		TextOut(hdc, (rwin.cx/2)-(int)(strlen(message)*3.25), (rwin.cy/2-120)+(20*0+40), message, strlen(message));
	}
}

int
update(hWnd)
HWND hWnd;
{
/*	int len=strlen(STATIC_MESSAGE)-1;*/

	colR = rand()%256;
	colG = rand()%256;
	colB = rand()%256;

	GetClientRect(hWnd, (LPRECT)&rWin);
	InvalidateRect(hWnd, (LPRECT)&rWin, TRUE);
	hdc = BeginPaint(hWnd, &ps);

	SetTextColor(hdc, RGB(colR, colG, colB));
	SetBkMode(hdc, TRANSPARENT);
/*	TextOut(hdc, (rWin.cx/2)-(int)(len*3.25), (rWin.cy/2)-20-50, STATIC_MESSAGE, strlen(STATIC_MESSAGE)+1);
 */

	textout_lines(NULL, 3, &rWin);
	EndPaint(hWnd, &ps);
	Sleep(200);
}

DWORD WINAPI
timer_thread(lpParameter)
LPVOID lpParameter;
{
	HWND hWnd = lpParameter;

	while(bActive) {
		update(hWnd);
	}

	return S_OK;
}

BOOL
hitwnd(rTest, point)
LPRECT rTest;
LPPOINT point;
{
	return (point->x < rTest->right &&
		point->x >= rTest->left &&
		point->y >= rTest->top &&
		point->y <= rTest->bottom);
}

LRESULT CALLBACK
main_procedure(hWnd, uMsg, wparam, lparam)
HWND hWnd;
UINT uMsg;
WPARAM wparam;
LPARAM lparam;
{
	switch (uMsg) {

	case WM_DESTROY:
		bActive = 0;
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
	case WM_LBUTTONDOWN: {
	} break;
	case WM_KEYDOWN: {
	} break;
	case WM_CTLCOLORSTATIC: {
		if((HWND)lparam == GetDlgItem(hWnd, IDC_STATIC)) {
			hdc = (HDC)wparam;
			SetBkColor(hdc, (COLORREF)GetSysColor(COLOR_WINDOW));
			SetTextColor(hdc, RGB(255, 0, 255));
			return (INT_PTR)GetSysColorBrush(COLOR_3DFACE);
		}
	} break;
	case WM_PAINT: {
		RECT rWnd;
		GetClientRect(hWnd, &rWnd);
		InvalidateRect(hWnd, &rWnd, TRUE);
		hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &rWnd, GetStockObject(COLOR_WINDOW+1));
		EndPaint(hWnd, &ps);
	} break;
	case WM_CREATE: {
		rect rWin;
		hdc = GetWindowDC(hWnd);
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
		MAINWND_SIZEX, MAINWND_SIZEY, NULL, NULL, mainWnd->hInst, NULL);
	if (!mainWnd->hWnd) {
		MessageBox(NULL, "Couldn't create the window sorry :P", "Error", MB_OK | MB_ICONERROR);
		return -2;
	}

	ShowWindow(mainWnd->hWnd, SW_SHOWNORMAL);
	UpdateWindow(mainWnd->hWnd);

	return 0;
}

void
keyboard_handler(mainWnd, hWndCaptured)
struct _tagMAINWND *mainWnd;
HWND *hWndCaptured;
{
	HWND hwndCaptured[256] = {NULL};
	bool bSwitch = false;
	hWndCaptured = &hwndCaptured[0];

	if ((GetAsyncKeyState(VK_F7) & 1) != 0) {
		static int i = 0;
		/*MessageBox(NULL, "Hiding window.", "Info", MB_OK|MB_ICONINFORMATION);*/
		if(i < 256) {
			if(hwndCaptured[i] == NULL) {
				hwndCaptured[i] = GetActiveWindow();
				ShowWindow(hwndCaptured[i], SW_HIDE);
				++i;
			}
		}
	}
	if ((GetAsyncKeyState(VK_F8) & 1) != 0) {
		static int i;
		/*MessageBox(NULL, "Showing all hidden windows.", "Info", MB_OK|MB_ICONINFORMATION);*/
		for(i = 0; i < 256; ++i)
			if(hwndCaptured[i] != NULL)
				ShowWindow(hwndCaptured[i], SW_SHOWNORMAL);
	}
	if((GetAsyncKeyState(VK_F9) & 1) != 0) {
		bSwitch = !bSwitch;
		if(!bSwitch)
			ShowWindow(mainWnd->hWnd, SW_SHOWNORMAL);
		else
			ShowWindow(mainWnd->hWnd, SW_HIDE);
	}
}

int
run(mainWnd)
struct _tagMAINWND *mainWnd;
{
	int i;
	HWND *hwndCapture = NULL;
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

		/* Handle keyboard and main event */
		keyboard_handler(mainWnd, hwndCapture);
	}

	if (WaitForSingleObject(timeThread.hThread, 1000))
		timeThread.bThreadAlive = FALSE;
/*
	for(i=0; i<255; ++i)
		if(hwndCapture[i] != NULL)
			ShowWindow(hwndCapture[i], SW_SHOW);
*/
	return (int)mainWnd->msg.wParam;
}