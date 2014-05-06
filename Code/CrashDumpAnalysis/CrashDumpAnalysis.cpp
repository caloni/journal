#include <windows.h>
#include "resource.h"

#define MNU_ACCESS_VIOLATION   10001
#define MNU_QUIT               10002
#define MNU_ABOUT              10003


HINSTANCE g_instance;
HWND g_mainWindow;


void AccessViolation()
{
	MessageBox(g_mainWindow, 
		(PCWSTR) 0x4567890, L"Access Violation", 
		MB_OK | MB_ICONERROR);
}


void About()
{
	MessageBox(g_mainWindow, 
		L"Crash Dump Analysis by Wanderley Caloni\r\n2010-07", L"About", 
		MB_OK | MB_ICONINFORMATION);
}


LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = 0;

	switch( msg )
	{
	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		case MNU_ACCESS_VIOLATION:
			AccessViolation();
			break;

		case MNU_ABOUT:
			About();
			break;

		case MNU_QUIT:
			PostQuitMessage(ERROR_SUCCESS);
			break;
		}
		break;

	case WM_CLOSE:
		PostQuitMessage(ERROR_SUCCESS);
		break;

	default:
		ret = DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return ret;
}


void MessageLoop()
{
	MSG msg;

	while( GetMessage(&msg, NULL, 0, 0) == TRUE )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


void InsertMenuSubItem(HMENU parent, PWSTR title = NULL, WORD id = 0)
{
	MENUITEMINFO menuItem = { sizeof(menuItem) };

	menuItem.fMask = MIIM_TYPE;

	if( title )
	{
		menuItem.fType = MFT_STRING;
		menuItem.dwTypeData = title;
		menuItem.cch = wcslen(menuItem.dwTypeData) + 1;
	}
	else menuItem.fType = MFT_MENUBARBREAK;

	if( id )
	{
		menuItem.fMask |= MIIM_ID;
		menuItem.wID = id;
	}

	InsertMenuItem(parent, id, FALSE, &menuItem);
}


void InsertMenuMainItem(HMENU parent, HMENU item, PWSTR title)
{
	MENUITEMINFO menuItem = { sizeof(menuItem) };
	
	menuItem.fMask = MIIM_TYPE | MIIM_SUBMENU;
	menuItem.hSubMenu = item;
	menuItem.fType = MFT_STRING;
	menuItem.dwTypeData = title;
	menuItem.cch = wcslen(menuItem.dwTypeData) + 1;
	
	InsertMenuItem(parent, 0, FALSE, &menuItem);
}


HMENU CreateMainMenu()
{
	HMENU ret = CreateMenu();

	if( ret )
	{
		HMENU crashItem = CreateMenu();
		InsertMenuSubItem(crashItem, L"Access Violation", MNU_ACCESS_VIOLATION);
		InsertMenuSubItem(crashItem, NULL);
		InsertMenuSubItem(crashItem, L"Quit", MNU_QUIT);

		HMENU helpItem = CreateMenu();
		InsertMenuSubItem(helpItem, L"About", MNU_ABOUT);


		InsertMenuMainItem(ret, helpItem, L"Help");
		InsertMenuMainItem(ret, crashItem, L"Crash");
	}

	return ret;
}


void CreateMainWindow()
{
	WNDCLASS mainClass = {};

	mainClass.hInstance = g_instance;
	mainClass.lpszClassName = L"CrashDumpAnalysis::MainClass";
	mainClass.lpfnWndProc = MainWindowProc;
	mainClass.hIcon = LoadIcon(g_instance, MAKEINTRESOURCE(IDI_ICON1));
	mainClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

	if( ATOM regClass = RegisterClass(&mainClass) )
	{
		HMENU mainMenu = CreateMainMenu();
		g_mainWindow = CreateWindow(L"CrashDumpAnalysis::MainClass", 
			L"Crash Dump Analysis", 
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 
			CW_USEDEFAULT, CW_USEDEFAULT, NULL, mainMenu, g_instance, 0);

		if( g_mainWindow )
		{
			ShowWindow(g_mainWindow, SW_SHOWDEFAULT);
			MessageLoop();
		}
	}
}


int WINAPI wWinMain(HINSTANCE inst, HINSTANCE, PWSTR, int)
{
	g_instance = inst;
	CreateMainWindow();
	return ERROR_SUCCESS;
}
