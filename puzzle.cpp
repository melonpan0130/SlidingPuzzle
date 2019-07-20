// game071301.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "game071301.h"
#include <time.h>
#include <math.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GAME071301, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME071301));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME071301));
	wcex.hCursor = LoadCursor(nullptr, IDC_CROSS); // 마우스 커서 모양 지정
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // 색상
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GAME071301);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, TEXT("퍼즐 게임"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
// wparam, &cursorx, &cursory, shwnd, xpos, ypos
bool MoveBlock(WPARAM wParam, int* cursorX, int* cursorY) {
	
	return false;
}

bool CheckBlock(int puzzle[24]) {
	for (int i = 0; i < 24; i++)
		if (puzzle[i] != i + 1)
			return false;
	return true;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND s_hwnd[5][5];
	static HWND s_hwnd2[5][5];
	static int s_nXPos[5][5];
	static int s_nYPos[5][5];
	static int s_CursorX;
	static int s_CursorY;

	static int puzzle[24];
	static int origin[24];

	srand((unsigned int)NULL);

	for (int i = 0; i < 24; i++) {
		puzzle[i] = i + 1;
		origin[i] = i + 1;
	}
	
	// mix the puzzle
	int key1, key2;
	for (int i = 0; i < 1000; i++) {
		key1 = (rand() % 24);
		key2 = (rand() % 24);

		int a = puzzle[key1];
		puzzle[key1] = puzzle[key2];
		puzzle[key2] = a;
	}

	switch (message)
	{
	case WM_CREATE:
	{
		s_CursorX = 4;
		s_CursorY = 4;

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++)
			{
				s_nXPos[i][j] = 10 + (j * 90);
				s_nYPos[i][j] = 10 + (i * 90);

				if (i == 4 && j == 4) {
					s_hwnd[i][j] = NULL;
					s_hwnd2[i][j] = NULL;
					break;
				}
				s_hwnd[i][j] = CreateWindow(TEXT("static"), NULL
					, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE
					, s_nXPos[i][j], s_nYPos[i][j], 80, 80, hWnd, (HMENU)-1
					, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
				
				TCHAR num[20];
				wsprintf(num, _T("%d"), puzzle[i * 5 + j]);
				SetWindowText(s_hwnd[i][j], num);
			}
		}
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		if (s_hwnd != NULL)

			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
				{
					if (s_hwnd[i][j] != NULL)
						DestroyWindow(s_hwnd[i][j]);
				}

		PostQuitMessage(0);
		break;
	case WM_KEYUP:
	{
		if (wParam == VK_RIGHT)
		{
			if (s_CursorX <= 3)
			{
				HWND hwnd = s_hwnd[s_CursorY][s_CursorX + 1];
				MoveWindow(hwnd, s_nXPos[s_CursorY][s_CursorX]
					, s_nYPos[s_CursorY][s_CursorX], 80, 80, TRUE);

				s_hwnd[s_CursorY][s_CursorX] = hwnd;
				s_hwnd[s_CursorY][s_CursorX + 1] = NULL;
				s_CursorX += 1;
			}
		}
		else if (wParam == VK_LEFT)
		{
			if (s_CursorX >= 1)
			{
				HWND hwnd = s_hwnd[s_CursorY][s_CursorX - 1];
				MoveWindow(hwnd, s_nXPos[s_CursorY][s_CursorX]
					, s_nYPos[s_CursorY][s_CursorX], 80, 80, TRUE);

				s_hwnd[s_CursorY][s_CursorX] = hwnd;
				s_hwnd[s_CursorY][s_CursorX - 1] = NULL;
				s_CursorX -= 1;
			}
		}
		else if (wParam == VK_UP)
		{
			if (s_CursorY >= 1)
			{
				HWND hwnd = s_hwnd[s_CursorY - 1][s_CursorX];
				MoveWindow(hwnd, s_nXPos[s_CursorY][s_CursorX]
					, s_nYPos[s_CursorY][s_CursorX], 80, 80, TRUE);

				s_hwnd[s_CursorY][s_CursorX] = hwnd;
				s_hwnd[s_CursorY - 1][s_CursorX] = NULL;
				s_CursorY -= 1;
			}
		}
		else if (wParam == VK_DOWN)
		{
			if (s_CursorY <= 3)
			{
				HWND hwnd = s_hwnd[s_CursorY + 1][s_CursorX];
				MoveWindow(hwnd, s_nXPos[s_CursorY][s_CursorX]
					, s_nYPos[s_CursorY][s_CursorX], 80, 80, TRUE);

				s_hwnd[s_CursorY][s_CursorX] = hwnd;
				s_hwnd[s_CursorY + 1][s_CursorX] = NULL;
				s_CursorY += 1;
			}
		}

		if (s_CursorX == 4 && s_CursorY == 4)
			CheckBlock(puzzle);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		
		int tileX = (x - 10) / 60;
		int tileY = (y - 10) / 60;

		if (tileX >= 0 && tileX < 5 && tileY >= 0 && tileY < 5) {
			int XDffset = s_CursorX - tileX;
			int YDffset = s_CursorY - tileY;

			if (abs(XDffset) <= 1 && abs(YDffset) <= 1
				&& abs(XDffset) + abs(YDffset) == 1)
			{
				int keyX = (XDffset == -1) ? VK_LEFT : (XDffset == 1) ? VK_RIGHT : 0;
				int keyY = (YDffset == -1) ? VK_UP : (YDffset == 1) ? VK_DOWN : 0;

				if (XDffset == -1)
				{ // left
					HWND hwnd = s_hwnd[s_CursorY][s_CursorX - 1];
					MoveWindow(hwnd, s_nXPos[s_CursorY][s_CursorX]
						, s_nYPos[s_CursorY][s_CursorX], 80, 80, TRUE);

					s_hwnd[s_CursorY][s_CursorX] = hwnd;
					s_hwnd[s_CursorY][s_CursorX - 1] = NULL;
					s_CursorX -= 1;
				}
				else if (XDffset == 1)
				{ // right
					HWND hwnd = s_hwnd[s_CursorY][s_CursorX + 1];
					MoveWindow(hwnd, s_nXPos[s_CursorY][s_CursorX]
						, s_nYPos[s_CursorY][s_CursorX], 80, 80, TRUE);

					s_hwnd[s_CursorY][s_CursorX] = hwnd;
					s_hwnd[s_CursorY][s_CursorX + 1] = NULL;
					s_CursorX += 1;
				}
				else if (YDffset == -1)
				{
					HWND hwnd = s_hwnd[s_CursorY - 1][s_CursorX];
					MoveWindow(hwnd, s_nXPos[s_CursorY][s_CursorX]
						, s_nYPos[s_CursorY][s_CursorX], 80, 80, TRUE);

					s_hwnd[s_CursorY][s_CursorX] = hwnd;
					s_hwnd[s_CursorY - 1][s_CursorX] = NULL;
					s_CursorY -= 1;
				}
				else if (YDffset == 1)
				{
					HWND hwnd = s_hwnd[s_CursorY + 1][s_CursorX];
					MoveWindow(hwnd, s_nXPos[s_CursorY][s_CursorX]
						, s_nYPos[s_CursorY][s_CursorX], 80, 80, TRUE);

					s_hwnd[s_CursorY][s_CursorX] = hwnd;
					s_hwnd[s_CursorY + 1][s_CursorX] = NULL;
					s_CursorY += 1;
				}
				HWND hwnd = s_hwnd[tileY][tileX];
				MoveWindow(hwnd, s_nXPos[s_CursorY][s_CursorX]
					, s_nYPos[s_CursorY][s_CursorX], 80, 80, TRUE);

				s_hwnd[s_CursorY][s_CursorX] = hwnd;
				s_hwnd[tileY][tileX] = NULL;

				s_CursorX = tileX;
				s_CursorY = tileY;
			}
		}
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
