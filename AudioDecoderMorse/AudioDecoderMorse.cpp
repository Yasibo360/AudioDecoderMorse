// Decoder Morze.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "AudioDecoderMorse.h"
#include "MorseСode.h"
#include "AudioRecorder.h"
#include "TextOperations.h"
#include "ControlsID.h"
#include <locale>

#define MAX_LOADSTRING 100
#define MAX_EDITSTRING 6000

// Глобальные переменные окна:
HINSTANCE hInst;                                // Текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // Имя класса главного окна
LPCWSTR szWndClassPane;

// Мои глобальные переменные
MorseСode morse;
AudioRecorder recorder(44100, 2, 16, 4096, "recorded.wav");

// Отправить объявления функций, включенных в этот модуль кода:
ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC WndProc, HBRUSH hbrBackground, LPCUWSTR szWindowClass);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProcPanes(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_AUDIODECODERMORSE, szWindowClass, MAX_LOADSTRING);

	szWndClassPane = L"Pane";

	// Регистрация классов
	MyRegisterClass(hInstance, WndProc, CreateSolidBrush(RGB(68, 121, 212)), szWindowClass);
	MyRegisterClass(hInstance, WndProcPanes, CreateSolidBrush(RGB(8, 111, 161)), szWndClassPane);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUDIODECODERMORSE));
	MSG msg;

	// Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC WndProc, HBRUSH hbrBackground, LPCUWSTR szWindowClass)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_AUDIODECODERMORSE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = hbrBackground;
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

/*
   ФУНКЦИЯ: InitInstance(HINSTANCE, int)

   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно

   КОММЕНТАРИИ:

		В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
		создается и выводится главное окно программы.
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

/*
  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)

  ЦЕЛЬ: Обрабатывает сообщения в главном окне.

  WM_COMMAND  - обработать меню приложения
  WM_PAINT    - Отрисовка главного окна
  WM_DESTROY  - отправить сообщение о выходе и вернуться
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rc;
	RECT menuButton;

	switch (message)
	{
	case WM_CREATE:
	{
		// Установка локали
		std::setlocale(LC_ALL, "");

		RECT separatorControls;

		rc.right = 1200;
		rc.bottom = 800;
		separatorControls.right = rc.right / 10;
		menuButton.right = separatorControls.right;
		menuButton.bottom = 50;

		// Вывод окна на середину экрана
		MoveWindow(hWnd, GetSystemMetrics(SM_CXSCREEN) / 2 - rc.right / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - rc.bottom / 2, rc.right, rc.bottom, true);

		GetClientRect(hWnd, &rc);

		// Создание меню навигации
		CreateWindowW(
			L"STATIC",
			L"",
			WS_VISIBLE | WS_CHILD | SS_GRAYFRAME,
			0,
			0,
			separatorControls.right,
			rc.bottom,
			hWnd,
			(HMENU)IDBgrPanes,
			hInst,
			nullptr);

		CreateWindowW(
			L"BUTTON",
			L"Главная",
			WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			0,
			rc.bottom / 2 - 2 * menuButton.bottom,
			separatorControls.right,
			menuButton.bottom,
			hWnd,
			(HMENU)IDButtPane1,
			hInst,
			nullptr);

		CreateWindowW(
			L"BUTTON",
			L"Расширенный режим",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			0,
			rc.bottom / 2 - menuButton.bottom,
			separatorControls.right,
			menuButton.bottom,
			hWnd,
			(HMENU)IDButtPane2,
			hInst,
			nullptr);

		CreateWindowW(
			L"BUTTON",
			L"Переводчик",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			0,
			rc.bottom / 2,
			separatorControls.right,
			menuButton.bottom,
			hWnd,
			(HMENU)IDButtPane3,
			hInst,
			nullptr);

		CreateWindowW(
			L"BUTTON",
			L"О программе",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			0,
			rc.bottom / 2 + menuButton.bottom,
			separatorControls.right,
			menuButton.bottom,
			hWnd,
			(HMENU)IDButtPane4,
			hInst,
			nullptr);

		// Создание панелей
		CreateWindow(
			szWndClassPane,
			L"",
			WS_VISIBLE | WS_CHILD,
			separatorControls.right, 0,
			rc.right, rc.bottom,
			hWnd,
			(HMENU)IDPane1,
			hInst,
			nullptr);

		CreateWindow(
			szWndClassPane,
			L"",
			WS_VISIBLE | WS_CHILD,
			separatorControls.right, 0,
			rc.right, rc.bottom,
			hWnd,
			(HMENU)IDPane2,
			hInst,
			nullptr);
		ShowWindow(GetDlgItem(hWnd, IDPane2), SW_HIDE);

		CreateWindow(
			szWndClassPane,
			L"",
			WS_VISIBLE | WS_CHILD,
			separatorControls.right, 0,
			rc.right, rc.bottom,
			hWnd,
			(HMENU)IDPane3,
			hInst,
			nullptr);
		ShowWindow(GetDlgItem(hWnd, IDPane3), SW_HIDE);

		CreateWindow(
			szWndClassPane,
			L"",
			WS_VISIBLE | WS_CHILD,
			separatorControls.right, 0,
			rc.right, rc.bottom,
			hWnd,
			(HMENU)IDPane4,
			hInst,
			nullptr);
		ShowWindow(GetDlgItem(hWnd, IDPane4), SW_HIDE);

		// Создание контролов для 1-ой панели
		CreateWindow(
			L"BUTTON",                          // Predefined class; Unicode assumed 
			L"Прослушать",                      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			(rc.right - separatorControls.right) / 2 - 75,// x position 
			rc.bottom / 2 - 75,                 // y position 
			150,                                // Button width
			150,                                // Button height
			GetDlgItem(hWnd, IDPane1),			// Parent window
			(HMENU)IDPane1ButtonRec,            // Child window
			hInst,
			nullptr);                           // Pointer not needed

		CreateWindowW(
			L"EDIT",
			L"",
			WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER,
			(rc.right - separatorControls.right) / 2 - 150,
			rc.bottom - 150 - 10,
			300,
			150,
			GetDlgItem(hWnd, IDPane1),
			(HMENU)IDPane1EditRes,
			hInst,
			nullptr);

		// Создание контролов для 2-ой панели


		// Создание контролов для 3-ой панели
		SIZE edit = {};
		SIZE button = {};
		edit.cx = 300;
		edit.cy = 300;
		button.cx = 35;
		button.cy = 35;

		CreateWindowW(
			L"EDIT",
			L"",
			WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER,
			(rc.right - separatorControls.right) / 2 - edit.cx - 10,
			rc.bottom / 2 - edit.cy,
			edit.cx,
			edit.cy,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3EditText,
			hInst,
			nullptr);
		SendMessageW(GetDlgItem(hWnd, IDPane3EditText), EM_LIMITTEXT, MAX_EDITSTRING / 6, 0);

		CreateWindowW(
			L"EDIT",
			L"",
			WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER,
			(rc.right - separatorControls.right) / 2 + 10,
			rc.bottom / 2 - edit.cy,
			edit.cx,
			edit.cy,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3EditCode,
			hInst,
			nullptr);
		SendMessageW(GetDlgItem(hWnd, IDPane3EditCode), EM_LIMITTEXT, MAX_EDITSTRING, 0);

		CreateWindow(
			L"BUTTON",
			L"Copy",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			(rc.right - separatorControls.right) / 2 - button.cx - 10,
			rc.bottom / 2 - (edit.cy + button.cy) - 10,
			button.cx,
			button.cy,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3ButtCopyText,
			hInst,
			nullptr);

		CreateWindow(
			L"BUTTON",
			L"Play",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			(rc.right - separatorControls.right) / 2 + (edit.cx - 2 * button.cx - 10) + 10,
			rc.bottom / 2 - (edit.cy + button.cy) - 10,
			button.cx,
			button.cy,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3ButtPlayCode,
			hInst,
			nullptr);

		CreateWindow(
			L"BUTTON",
			L"Copy",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			(rc.right - separatorControls.right) / 2 + (edit.cx - button.cx) + 10,
			rc.bottom / 2 - (edit.cy + button.cy) - 10,
			button.cx,
			button.cy,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3ButtCopyCode,
			hInst,
			nullptr);

		// Создание контролов для 4-ой панели
		ShowWindow(GetDlgItem(hWnd, IDPane1), SW_SHOW);
		UpdateWindow(hWnd);
	}
	break;
	case WM_COMMAND:
	{
		switch HIWORD(wParam)
		{
		case BN_CLICKED:
		{
			switch (LOWORD(wParam))
			{
			case IDButtPane1:
			{
				ShowWindow(GetDlgItem(hWnd, IDPane2), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane3), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane4), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane1), SW_SHOW);
			}
			break;
			case IDButtPane2:
			{
				ShowWindow(GetDlgItem(hWnd, IDPane1), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane3), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane4), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane2), SW_SHOW);
			}
			break;
			case IDButtPane3:
			{
				ShowWindow(GetDlgItem(hWnd, IDPane1), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane2), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane4), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane3), SW_SHOW);
			}
			break;
			case IDButtPane4:
			{
				ShowWindow(GetDlgItem(hWnd, IDPane1), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane2), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane3), SW_HIDE);
				ShowWindow(GetDlgItem(hWnd, IDPane4), SW_SHOW);
			}
			break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		}
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndProcPanes(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		WCHAR buffer[MAX_EDITSTRING];
		std::wstring morseCode;
		std::wstring morseChar;

		switch HIWORD(wParam)
		{
		case BN_CLICKED:
		{
			switch LOWORD(wParam)
			{
			case IDPane3ButtCopyText:
			{
				CopyIntoBuffer(hWnd, IDPane3EditText);
			}
			break;
			case IDPane3ButtPlayCode:
			{
				GetWindowTextW(GetDlgItem(hWnd, IDPane3EditCode), buffer, MAX_LOADSTRING);
				for (size_t i = 0; (i < sizeof(buffer) / 2 - 1) && (buffer[i] != 0); i++)
				{
					morseCode += buffer[i];
				}
				morse.PlayMorseCode(morseCode);
			}
			break;
			case IDPane3ButtCopyCode:
			{
				CopyIntoBuffer(hWnd, IDPane3EditCode);
			}
			break;
			case IDPane1ButtonRec:
			{
				if (!recorder.IsRecording()) {
					recorder.StartRecording();
				}
				else {
					recorder.StopRecording();
					// Добавить расшифровку аудиосообщения из записанного файла

				}
			}
			break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		case EN_CHANGE:
		{
			switch LOWORD(wParam)
			{
			case IDPane3EditText:
			{
				if (GetWindowTextLength(GetDlgItem(hWnd, IDPane3EditText)) < SendMessageW(GetDlgItem(hWnd, IDPane3EditText), EM_GETLIMITTEXT, 0, 0) - 10)
				{
					GetWindowTextW(GetDlgItem(hWnd, IDPane3EditText), buffer, MAX_EDITSTRING);
					for (size_t i = 0; (i < sizeof(buffer) / 2 - 1) && (buffer[i] != 0); i++)
					{
						morseChar += buffer[i];
					}
					morseCode = morse.CharToMorse(morseChar);
					if (morseCode.length() != 0)
					{
						for (size_t i = 0; i < morseCode.length() - 1; i++)
						{
							buffer[i] = morseCode[i];
							if (i == morseCode.length() - 2)
							{
								buffer[i + 1] = '\0';
							}
						}
						SetWindowTextW(GetDlgItem(hWnd, IDPane3EditCode), buffer);
					}
					else
					{
						SetWindowTextW(GetDlgItem(hWnd, IDPane3EditCode), 0);
					}
				}
			}
			break;
			case IDPane3EditCode:
			{
				if (SendMessageW(GetDlgItem(hWnd, IDPane3EditCode), EM_LINELENGTH, 0, 0) < SendMessageW(GetDlgItem(hWnd, IDPane3EditCode), EM_GETLIMITTEXT, 0, 0))
				{

				}
			}
			break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		case EN_ERRSPACE:
		{

		}
		case EN_MAXTEXT:
		{

		}
		}
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}