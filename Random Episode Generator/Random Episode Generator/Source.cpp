#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#include "App.h"
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>

// Global variables
static TCHAR szWindowClass[] = _T("RandomEpisodeGenerator");							// The main window class name.
static TCHAR szTitle[] = _T("Lark Random Episode Generator");							// The string that appears in the application's title bar.
HINSTANCE hInst;																		// Stored instance handle for use in Win32 API calls such as FindResource
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);									// Forward declarations of functions included in this code module:
int episodesToShow = 0;																	// For dynamic showing of the list
int priorEpisodesToShow = 0;															// Used to properly clear the background

// Set background colors
int bkR = 0;
int bkG = 55;
int bkB = 0;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	// Store instance handle in our global variable
	hInst = hInstance;

	// Set the main window parameters
	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1280, 720,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Mark's Random Episode Generator"),
			NULL);

		return 1;
	}

	// Initialize the application
	mainApp();

	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}	

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Mark's Random Episode Generator");
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));

	bool clearListBackground = false;
	int numToShow = getNumberToShow();
	std::vector<std::string> myEpisodesList;

	int ListRectTopPos = 100;
	int ListRectLeftPos = 500;
	int ListRectRightPos = ListRectLeftPos + 600;
	int ListRectBottomPos = 500;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SetTextColor(hdc, RGB(0, 255, 0));
		SetBkColor(hdc, RGB(0, 0, 0));
		
		if (episodesToShow > numToShow) {
			clearListBackground = true;
			priorEpisodesToShow = episodesToShow;
		}
		episodesToShow = numToShow;
		myEpisodesList = showRecentEpisodes(episodesToShow);

		//int numberOfRecentEpisodesSize = 0;
		
		// Clear the area where the list is displayed
		RECT rect;
		
		if (clearListBackground) {
			HBRUSH hBrushTemp = CreateSolidBrush(RGB(bkR, bkG, bkB));
			rect.left = ListRectLeftPos;
			rect.top = ListRectTopPos + (20 * episodesToShow);
			rect.right = ListRectRightPos;
			rect.bottom = ListRectTopPos + (20 * priorEpisodesToShow);

			FillRect(hdc, &rect, hBrushTemp);
			DeleteObject(hBrushTemp);
		}
		
		rect.left = ListRectLeftPos;
		rect.top = 100;
		rect.right = ListRectRightPos;
		rect.bottom = rect.top + (20 * episodesToShow);
			
		
		
		FillRect(hdc, &rect, hBrush); // Fill the area with the brush
		DeleteObject(hBrush); // Delete the brush

		// Lay out the application
		for (int i = 0; i < episodesToShow; i++) {
			std::string episodeList = myEpisodesList[i];
			std::vector<TCHAR> buffer(episodeList.begin(), episodeList.end());
			buffer.push_back(_T('\0'));													// Zero terminate the buffer
			TCHAR* newEpisodeList = &buffer[0];


			if (episodeList != "" || episodeList != "ALL EPISODES VIEWED:") {
				TextOut(hdc,
					500, 100 + (20 * i),
					newEpisodeList, _tcslen(newEpisodeList));
			}
		}
		
		// End application-specific layout section.

		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND: {
		HDC hdc = (HDC)(wParam);
		RECT rc; 
		GetClientRect(hWnd, &rc);
		HBRUSH brush = CreateSolidBrush(RGB(bkR, bkG, bkB));							// Light green background
		FillRect(hdc, &rc, brush);
		DeleteObject(brush);
		break;
	}
	case WM_CREATE:
	{
		CreateWindowEx(0, TEXT("BUTTON"), TEXT("Play Random\n\tEpisode"),
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_MULTILINE,
			20, 50, 150, 90,
			hWnd, (HMENU)1, NULL, NULL);

		CreateWindow(TEXT("button"), TEXT("Quit"),
			WS_VISIBLE | WS_CHILD,
			20, 550, 80, 40,
			hWnd, (HMENU)2, NULL, NULL);

		CreateWindow(TEXT("button"), TEXT("Show 5 Episodes"),
			WS_VISIBLE | WS_CHILD,
			ListRectLeftPos, ListRectTopPos - 50, 160, 40,
			hWnd, (HMENU)3, NULL, NULL);

		CreateWindow(TEXT("button"), TEXT("Show 10 Episodes"),
			WS_VISIBLE | WS_CHILD,
			ListRectLeftPos + 200, ListRectTopPos - 50, 160, 40,
			hWnd, (HMENU)4, NULL, NULL);

		CreateWindow(TEXT("button"), TEXT("Light Mode"),
			WS_VISIBLE | WS_CHILD,
			ListRectLeftPos + 350, ListRectTopPos + 500, 160, 40,
			hWnd, (HMENU)5, NULL, NULL);

		CreateWindow(TEXT("button"), TEXT("Dark Mode"),
			WS_VISIBLE | WS_CHILD,
			ListRectLeftPos + 550, ListRectTopPos + 500, 160, 40,
			hWnd, (HMENU)6, NULL, NULL);

		
		break;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == 1) {
			Beep(40, 50);
		}

		if (LOWORD(wParam) == 2) {
			PostQuitMessage(0);
		}

		if (LOWORD(wParam) == 3) {
			InvalidateRect(
				hWnd,
				NULL,
				NULL);
			clearWatchedList();
			setNumberToShow(5);
			updateRecentWatched();
		}

		if (LOWORD(wParam) == 4) {
			InvalidateRect(
				hWnd,
				NULL,
				NULL);
			clearWatchedList();
			setNumberToShow(10);
			updateRecentWatched();
		}

		if (LOWORD(wParam) == 5) {					// Light Mode
			bkR = 200;
			bkG = 255;
			bkB = 200;
			
			HDC hdc = (HDC)(wParam);
			RECT rc;
			GetClientRect(hWnd, &rc);
			HBRUSH brush = CreateSolidBrush(RGB(bkR, bkG, bkB));							// Light green background
			FillRect(hdc, &rc, brush);
			DeleteObject(brush);
		}

		if (LOWORD(wParam) == 6) {					// Dark Mode
			bkR = 0;
			bkG = 55;
			bkB = 0;
			
			HDC hdc = (HDC)(wParam);
			RECT rc;
			GetClientRect(hWnd, &rc);
			HBRUSH brush = CreateSolidBrush(RGB(bkR, bkG, bkB));							// Light green background
			FillRect(hdc, &rc, brush);
			DeleteObject(brush);
		}



		break;
	}
	case WM_DESTROY:
		PostQuitMessage(1);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	myEpisodesList.clear();
	return 0;
}