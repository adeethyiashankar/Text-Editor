// TextEditor.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "TextEditor.h"
#include <ShObjIdl.h>
#include <shellapi.h>
#include <CommDlg.h>
#include <fstream>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND Start_Button;								// Start button handle
HWND hTitle;									// Title handle
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
LPWSTR pszFilePath;								// Selected file path
bool winXP = false;
bool showInExplorer = false;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void OpenFileWinXP(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	LPWSTR* szArgList;
	int argCount;
	szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
	if (argCount > 1)
	{
		pszFilePath = szArgList[1];
		if (pszFilePath != L"")
		{
			int choice = MessageBoxW(0, L"Are you sure you would like to destroy this file?", L"Warning!", MB_ICONWARNING | MB_YESNO);
			if (choice == IDNO)
			{}
			else
			{
				std::fstream del;
				del.open(std::wstring(pszFilePath).c_str(), std::ios::out | std::ios::trunc);
				if (!del.is_open())
				{
					MessageBoxW(0, L"File cannot be opened", L"Error", MB_ICONERROR);
					showInExplorer = false;
					return 0;
				}
				del << "Destroyed with Adeethyia Shankar's Text Editor";
				del.close();
				MessageBoxW(0, L"The file is destroyed.", L"Done", MB_ICONEXCLAMATION | MB_OK);
				return 0;
			}
		}
	}
	LocalFree(szArgList);
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TEXTEDITOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEXTEDITOR));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ADEETHYIA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TEXTEDITOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ADEETHYIA));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT window;
	GetClientRect(hWnd, &window);
	switch (message)
	{
	case WM_COMMAND:
	{
		// Parse the menu selections:
		switch (LOWORD(wParam))
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_START:
		{
			int choice = MessageBoxW(hWnd, L"If you open a file in the file chooser, it will be destroyed. Do you want to proceed?", L"Warning!", MB_ICONWARNING | MB_YESNO);
			if (choice == IDNO)
				break;
			OpenFileWinXP(hWnd);
			break;
		}
		case IDM_HELP:
		{
			int choice = MessageBoxW(hWnd, L"Destroy a file by replacing the contents of the file with the phrase \"Destroyed with Adeethyia Shankar's Text Editor\"\n\nPress OK to start", L"Help", MB_OKCANCEL);
			if (choice == IDCANCEL)
				break;
			SendMessage(hWnd, (UINT)WM_COMMAND, (WPARAM)IDM_START, (LPARAM)0);
			break;
		}
		}
		break;
	case WM_CREATE:
		Start_Button = CreateWindowExW(0L, L"Button", L"Start", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, window.bottom / 2, window.right, window.bottom / 2, hWnd, (HMENU)IDM_START, NULL, NULL);
		hTitle = CreateWindowExW(0L, L"static", L"Text Editor", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 0, window.right, (int)window.bottom / 2, hWnd, NULL, NULL, NULL);
		break;
	case WM_PAINT:
	{
		int length = window.right > window.bottom ? window.bottom : window.right;
		int font = (int)240 * length / 534;
		if (font > 200)
			font = 200;
		else if (font > 100)
			font = 100;
		else if (font > 50)
			font = 50;
		else if (font > 30)
			font = 30;
		HFONT hFont = CreateFont(font, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, UNICODE, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Calibri");
		MoveWindow(Start_Button, 0, (int)window.bottom / 2, window.right, (int)window.bottom / 2, true);
		MoveWindow(hTitle, 0, 0, window.right, (int)window.bottom / 2, true);
		SendMessage(Start_Button, WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(hTitle, WM_SETFONT, WPARAM(hFont), TRUE);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == hTitle)
		{
			HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(0, 0, 0));
			SetBkColor(hdcStatic, RGB(255, 255, 255));
			return (INT_PTR)hBrush;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
	}
}

void OpenFileWinXP(HWND hWnd)
{
	wchar_t szFile[1024];
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = NULL;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	if (GetOpenFileName(&ofn))
	{
		pszFilePath = ofn.lpstrFile;
		int choice = MessageBoxW(hWnd, L"Are you sure you would like to destroy this file?", L"Warning!", MB_ICONWARNING | MB_YESNO);
		if (choice == IDNO)
			return;
		std::fstream del;
		del.open(std::wstring(pszFilePath).c_str(), std::ios::out | std::ios::trunc);
		if (!del.is_open())
		{
			MessageBoxW(hWnd, L"File cannot be opened", L"Error", MB_ICONERROR);
			showInExplorer = false;
			return;
		}
		del << "Destroyed with Adeethyia Shankar's Text Editor";
		del.close();
		MessageBoxW(hWnd, L"The file is destroyed.", L"Done", MB_ICONEXCLAMATION | MB_OK);
		showInExplorer = true;
		if (showInExplorer)
		{
			std::wstring check = std::wstring(pszFilePath);
			unsigned long index = check.find_last_of(L"\\");
			std::wstring dir = check.substr(0, index);
			ShellExecute(0, 0, L"C:\\Windows\\explorer.exe", dir.c_str(), 0, SW_SHOW);
		}
		pszFilePath = nullptr;
	}
}