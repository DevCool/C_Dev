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
		while(PeekMessage(&mainWnd.uMsg, NULL, 0, 0, PM_REMOVE) != 0) {
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
		case WM_CREATE:
			if(btnControl(&hwndButton1, hwnd, GetModuleHandle(NULL), "&Connect",
				20, 20, 175, 20, (HMENU)IDC_BUTTON1) < 0)
				MessageBox(hwnd, "Cannot create sexy button.", "Error", MB_OK|MB_ICONERROR);
			if(btnControl(&hwndButton2, hwnd, GetModuleHandle(NULL), "&Send",
				640-150, 480-60, 130, 20, (HMENU)IDC_BUTTON2) < 0)
				MessageBox(hwnd, "Cannot create send button.", "Error", MB_OK|MB_ICONERROR);
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
	*hWnd = CreateWindow("Edit", lpCaption, WS_VISIBLE | WS_CHILD,
		x, y, x2, y2, hWndParent, hMenu, hInst, NULL);
	if(*hWnd == NULL)
		return -1;
	return 0;
}
