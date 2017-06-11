/****************************************************
 * SAB 2 Project                                    *
 ****************************************************
 * Copyright (C) 2017 Philip Simonson.              *
 ****************************************************
 * Do NOT use this for malicious activities, no     *
 * "hacking" or "cracking" with this software.      *
 ****************************************************
 */

#if !defined(helper_h)
#define helper_h

#if defined(_WIN32) || (_WIN64)
#include <winsock2.h>
#include <windows.h>

typedef struct _tagBASEWND CWND, *LPCWND;
struct _tagBASEWND {
	HWND hWindow;
	HINSTANCE hInstance;
	MSG uMsg;
	BOOL bRunning;
};

int run_code(int *argc, char **argv[]);
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT);
int btnControl(HWND *hWnd, HWND hWndParent, HINSTANCE hInst, LPSTR lpCaption,
	int x, int y, int x2, int y2, HMENU hMenu);
int txtControl(HWND *hWnd, HWND hWndParent, HINSTANCE hInst, LPSTR lpCaption,
	int x, int y, int x2, int y2, HMENU hMenu);

#endif
#endif
