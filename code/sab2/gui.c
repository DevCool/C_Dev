/****************************************************
 * SAB 2 Project                                    *
 ****************************************************
 * Copyright (C) 2017 Philip Simonson.              *
 ****************************************************
 * Do NOT use this for malicious activities, no     *
 * "hacking" or "cracking" with this software.      *
 ****************************************************
 */

#include <stdio.h>
#include "helper.h"

#define IDC_BUTTON1 1001
#define IDC_BUTTON2 1002
#define IDC_EDIT1 1003

CWND mainWnd;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nShowCmd)
{
	WNDCLASSEX wc;
	ZeroMemory(&mainWnd, sizeof(mainWnd));
	ZeroMemory(&wc, sizeof(wc));
	mainWnd.hInstance = hInst;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.hInstance = mainWnd.hInstance;
	wc.lpfnWndProc = (WNDPROC)WindowProcedure;
	wc.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW+1);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "MainWndClass";
	wc.lpszMenuName = (LPCSTR)NULL;
	wc.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
	if(RegisterClassEx(&wc) == 0)
		return 0;

	mainWnd.hWindow = CreateWindowEx(
		0,
		"MainWndClass",
		"SAB 2",
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		NULL,
		NULL,
		mainWnd.hInstance,
		NULL);
	if(mainWnd.hWindow == NULL)
		return 0;

	ShowWindow(mainWnd.hWindow, SW_SHOWNORMAL);
	UpdateWindow(mainWnd.hWindow);

	mainWnd.bRunning = 1;
	while(mainWnd.bRunning) {
		if(PeekMessage(&mainWnd.uMsg, NULL, 0, 0, PM_REMOVE) != 0) {
			TranslateMessage(&mainWnd.uMsg);
			DispatchMessage(&mainWnd.uMsg);
		}

		if(mainWnd.uMsg.message == WM_QUIT)
			mainWnd.bRunning = 0;

		/* TODO: Add more code here */
	}
	return (int)mainWnd.uMsg.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hwndButton1;
	HWND hwndButton2;
	HWND hwndText1;
	DWORD dwTimer1;
	BOOL bTest = FALSE;

	switch(msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		case WM_QUIT:
			ExitProcess(0);
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_COMMAND:
			if(HIWORD(wParam) == 0) {
			}
			if(LOWORD(wParam) == IDC_BUTTON1) {
				dwTimer1 = GetTickCount();
				SetWindowText(hwndText1, "Connected!");
				while((GetTickCount()-dwTimer1) > 3);
				ShowWindow(hwndButton1, SW_HIDE);
				ShowWindow(hwndText1, SW_HIDE);
				ShowWindow(hwndButton2, SW_SHOW);
			}
		break;
		case WM_KEYDOWN:
			if(wParam == VK_F5) {
				bTest = !bTest;
				if(bTest) {
					dwTimer1 = GetTickCount();
					SetWindowText(hwndText1, "Connected!");
					while((GetTickCount()-dwTimer1) > 3);
					ShowWindow(hwndButton1, SW_HIDE);
					ShowWindow(hwndText1, SW_HIDE);
					ShowWindow(hwndButton2, SW_SHOW);
				} else {
					SetWindowText(hwndText1, "Enter IP Address");
					ShowWindow(hwndButton1, SW_SHOW);
					ShowWindow(hwndText1, SW_SHOW);
					ShowWindow(hwndButton2, SW_HIDE);
				}
			}
		break;
		case WM_CREATE:
			if(btnControl(&hwndButton1, hwnd, GetModuleHandle(NULL), "&Connect",
				(640/2)-(175/2), (480/2), 175, 20, (HMENU)IDC_BUTTON1) < 0)
				MessageBox(hwnd, "Cannot create sexy button.", "Error", MB_OK|MB_ICONERROR);
			if(btnControl(&hwndButton2, hwnd, GetModuleHandle(NULL), "&Send",
				640-150, 480-60, 130, 20, (HMENU)IDC_BUTTON2) < 0)
				MessageBox(hwnd, "Cannot create send button.", "Error", MB_OK|MB_ICONERROR);
			if(txtControl(&hwndText1, hwnd, GetModuleHandle(NULL), "Enter IP Address",
				(640/2)-(175/2), (480/2)-30, 175, 20, (HMENU)IDC_EDIT1) < 0)
				MessageBox(hwnd, "Cannot create text1 control.", "Error", MB_OK|MB_ICONERROR);
			ShowWindow(hwndButton2, SW_HIDE);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

int btnControl(HWND *hWnd, HWND hWndParent, HINSTANCE hInst, LPSTR lpCaption,
	int x, int y, int x2, int y2, HMENU hMenu)
{
	*hWnd = CreateWindow("Button", lpCaption, WS_VISIBLE | WS_CHILD,
		x, y, x2, y2, hWndParent, hMenu, hInst, NULL);
	if(*hWnd == NULL)
		return -1;
	return 0;
}

int txtControl(HWND *hWnd, HWND hWndParent, HINSTANCE hInst, LPSTR lpCaption,
	int x, int y, int x2, int y2, HMENU hMenu)
{
	*hWnd = CreateWindow("Edit", lpCaption, WS_VISIBLE | WS_CHILD | WS_BORDER | BS_FLAT,
		x, y, x2, y2, hWndParent, hMenu, hInst, NULL);
	if(*hWnd == NULL)
		return -1;
	return 0;
}
