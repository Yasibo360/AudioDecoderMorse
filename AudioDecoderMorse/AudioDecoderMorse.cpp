// Decoder Morze.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "AudioDecoderMorse.h"

#include "MorseСode.h"
#include "AudioRecorder.h"
#include "TextOperations.h"
#include "ControlsID.h"
#include "ControlsRect.h"

#include <locale>

#define MAX_LOADSTRING 100
#define MAX_EDITSTRING 6000

// Глобальные переменные окна:
HINSTANCE hInst;                                // Текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // Имя класса главного окна
LPCWSTR szWndClassPane;

HWND hWndComboBox;
HWND hWndPane1;
HWND hWndPane2;
HWND hWndPane3;
HWND hWndPane4;

// Мои глобальные переменные
MorseСode morse;
AudioRecorder recorder("recorded.wav");

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
	MyRegisterClass(hInstance, WndProc, CreateSolidBrush(RGB(16, 16, 16)), szWindowClass);
	MyRegisterClass(hInstance, WndProcPanes, CreateSolidBrush(RGB(36, 36, 36)), szWndClassPane);

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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

// Инициализация глобальных переменных
void InitControlsRect(RECT rc)
{
	rectMenu = { rc.left, rc.top, rc.right / part, rc.bottom };
	sizeMenuButton = { rectMenu.right, 50 };

	rectPage = { rectMenu.right, rc.top, rc.right - rectMenu.right, rc.bottom };

	// Размеры контролов для главной страницы
	rectPane1ButtonRec = {
		(rectPage.right - 150) / 2,
		(rectPage.bottom - 150) / 2,
		150,
		150
	};
	rectPane1EditRes = {
		(rectPage.right - 600) / 2,
		(rectPage.bottom - 200) - 2 * sizeIndentation.cy,
		600,
		200
	};
	// Размеры контролов для страницы редактирования

	// Размеры контролов для страницы переводчика
	int indOnTopPane3 = 8;

	rectPane3EditText = {
		rectPage.right / 2 - 300 - 2 * sizeIndentation.cx,
		rectPage.top + indOnTopPane3 * sizeIndentation.cy,
		300,
		300
	};
	rectPane3EditCode = {
		rectPage.right / 2 + 2 * sizeIndentation.cx,
		rectPage.top + indOnTopPane3 * sizeIndentation.cy,
		300,
		300
	};
	rectPane3ButtCopyText = {
		rectPane3EditText.left + rectPane3EditText.right - sizeButton.cx,
		rectPane3EditText.top + rectPane3EditText.bottom + sizeIndentation.cy,
		sizeButton.cx,
		sizeButton.cy
	};
	rectPane3ButtPlayCode = {
		rectPane3EditCode.left + rectPane3EditCode.right - 2 * sizeButton.cx - sizeIndentation.cx,
		rectPane3EditCode.top + rectPane3EditCode.bottom + sizeIndentation.cy,
		sizeButton.cx,
		sizeButton.cy
	};
	rectPane3ButtCopyCode = {
		rectPane3EditCode.left + rectPane3EditCode.right - sizeButton.cx,
		rectPane3EditCode.top + rectPane3EditCode.bottom + sizeIndentation.cy,
		sizeButton.cx,
		sizeButton.cy
	};
	rectPane3ListDict = {
		rectPage.right / 2 - 3 * sizeButton.cx,
		rectPane3ButtPlayCode.top + rectPane3ButtPlayCode.bottom + 5 * sizeIndentation.cy,
		6 * sizeButton.cx,
		4 * sizeButton.cy
	};
	rectPane3ButtAddDic = {
		rectPane3ListDict.left + rectPane3ListDict.right + sizeIndentation.cx,
		rectPane3ButtPlayCode.top + rectPane3ButtPlayCode.bottom + 5 * sizeIndentation.cx,
		sizeButton.cx,
		sizeButton.cy
	};

	// Размеры контролов для страницы описания
	rectPane4Picture = {
		rectPage.right / 2 + sizeIndentation.cx,
		rectPage.top + 10 * sizeIndentation.cy,
		200,
		300
	};

	return;
}

// =====================================
// Функция DrawButton
// Перерисовывает кнопку
// =====================================

void DrawButton(LPDRAWITEMSTRUCT lpInfo, HBRUSH hBrush)
{
	TCHAR szBuffer[MAX_LOADSTRING]; // Буфер для хранения загруженной строки
	int countChar = 0;

	// Обрабатываем сообщение WM_DRAWITEM
	// только если оно поступило от кнопки 
	if (lpInfo->CtlType != ODT_BUTTON) return;

	switch (lpInfo->CtlID)
	{
	case IDButtPane1:
	{
		LoadStringW(hInst, IDS_MenuPage1, szBuffer, MAX_LOADSTRING);
		countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
	}
	break;
	case IDButtPane2:
	{
		LoadStringW(hInst, IDS_MenuPage2, szBuffer, MAX_LOADSTRING);
		countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
	}
	break;
	case IDButtPane3:
	{
		LoadStringW(hInst, IDS_MenuPage3, szBuffer, MAX_LOADSTRING);
		countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
	}
	break;
	case IDButtPane4:
	{
		LoadStringW(hInst, IDS_MenuPage4, szBuffer, MAX_LOADSTRING);
		countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
	}
	break;
	default:
		return;
	}

	FillRect(lpInfo->hDC, &lpInfo->rcItem, hBrush);

	// Если кнопка выбрана, рисуем ее в нажатом
	// состоянии
	if (lpInfo->itemState & ODS_SELECTED)
	{
		FillRect(lpInfo->hDC, &lpInfo->rcItem, hBrush);
		FillRect(lpInfo->hDC, &lpInfo->rcItem, hBrush);
		HBRUSH hBrushPressed = CreateSolidBrush(RGB(60, 60, 60));
		InflateRect(&lpInfo->rcItem, -4, -4);
		FillRect(lpInfo->hDC, &lpInfo->rcItem, hBrushPressed);
		DeleteObject(hBrushPressed);
	}

	// Если кнопка выбрана и ее надо целиком
	// перерисовать, вызываем функцию DrawBitmap
	if ((lpInfo->itemAction & ODA_DRAWENTIRE) ||
		(lpInfo->itemAction & ODA_SELECT))
	{
		// Отрисовка текста кнопки
		SetBkMode(lpInfo->hDC, TRANSPARENT);
		SetTextColor(lpInfo->hDC, RGB(255, 255, 255));
		DrawTextW(lpInfo->hDC, szBuffer, countChar, &lpInfo->rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
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
	static HBRUSH hBrush;

	switch (message)
	{
	case WM_CREATE:
	{
		// Установка локали
		std::setlocale(LC_ALL, "");

		const SIZE sizeWnd = { 1200, 800 };
		TCHAR szBuffer[MAX_LOADSTRING]; // Буфер для хранения загруженной строки
		int countChar = 0;

		// Вывод окна на середину экрана
		MoveWindow(hWnd, GetSystemMetrics(SM_CXSCREEN) / 2 - sizeWnd.cx / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - sizeWnd.cy / 2, sizeWnd.cx, sizeWnd.cy, true);

		GetClientRect(hWnd, &rc);

		InitControlsRect(rc);

		/*
		  Создание меню навигации
		*/
		CreateWindowW(
			L"STATIC",
			L"",
			WS_VISIBLE | WS_CHILD | SS_SIMPLE,
			rc.left,
			rc.top,
			rc.right / part,
			rc.bottom,
			hWnd,
			(HMENU)IDBgrPanes,
			hInst,
			nullptr);

		hBrush = CreateSolidBrush(RGB(40, 40, 40)); // Создание кисти для фона кнопки

		CreateWindowW(
			L"BUTTON",
			L"Главная",
			WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
			0,
			rectMenu.bottom / 2 - 2 * sizeMenuButton.cy,
			sizeMenuButton.cx,
			sizeMenuButton.cy,
			hWnd,
			(HMENU)IDButtPane1,
			hInst,
			nullptr);
		InvalidateRect(GetDlgItem(hWnd, IDButtPane1), NULL, FALSE);
		UpdateWindow(GetDlgItem(hWnd, IDButtPane1));

		CreateWindowW(
			L"BUTTON",
			L"Расширенный режим",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
			0,
			rectMenu.bottom / 2 - sizeMenuButton.cy,
			sizeMenuButton.cx,
			sizeMenuButton.cy,
			hWnd,
			(HMENU)IDButtPane2,
			hInst,
			nullptr);

		CreateWindowW(
			L"BUTTON",
			L"Переводчик",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
			0,
			rectMenu.bottom / 2,
			sizeMenuButton.cx,
			sizeMenuButton.cy,
			hWnd,
			(HMENU)IDButtPane3,
			hInst,
			nullptr);

		CreateWindowW(
			L"BUTTON",
			L"О программе",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
			0,
			rectMenu.bottom / 2 + sizeMenuButton.cy,
			sizeMenuButton.cx,
			sizeMenuButton.cy,
			hWnd,
			(HMENU)IDButtPane4,
			hInst,
			nullptr);

		/*
		  Создание панелей
		*/
		hWndPane1 = CreateWindow(
			szWndClassPane,
			L"",
			WS_VISIBLE | WS_CHILD,
			rectPage.left,
			rectPage.top,
			rectPage.right,
			rectPage.bottom,
			hWnd,
			(HMENU)IDPane1,
			hInst,
			nullptr);

		hWndPane2 = CreateWindow(
			szWndClassPane,
			L"",
			WS_VISIBLE | WS_CHILD,
			rectPage.left,
			rectPage.top,
			rectPage.right,
			rectPage.bottom,
			hWnd,
			(HMENU)IDPane2,
			hInst,
			nullptr);
		ShowWindow(hWndPane2, SW_HIDE);

		hWndPane3 = CreateWindow(
			szWndClassPane,
			L"",
			WS_VISIBLE | WS_CHILD,
			rectPage.left,
			rectPage.top,
			rectPage.right,
			rectPage.bottom,
			hWnd,
			(HMENU)IDPane3,
			hInst,
			nullptr);
		ShowWindow(hWndPane3, SW_HIDE);

		hWndPane4 = CreateWindow(
			szWndClassPane,
			L"",
			WS_VISIBLE | WS_CHILD,
			rectPage.left,
			rectPage.top,
			rectPage.right,
			rectPage.bottom,
			hWnd,
			(HMENU)IDPane4,
			hInst,
			nullptr);
		ShowWindow(hWndPane4, SW_HIDE);

		/*
		  Создание контролов для 1-ой панели
		*/

		// Создание кнопки прослушивания
		CreateWindowW(
			L"BUTTON",												// Predefined class; Unicode assumed 
			L"Прослушать",											// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			rectPane1ButtonRec.left,								// x position 
			rectPane1ButtonRec.top,									// y position 
			rectPane1ButtonRec.right,								// Button width
			rectPane1ButtonRec.bottom,								// Button height
			GetDlgItem(hWnd, IDPane1),								// Parent window
			(HMENU)IDPane1ButtonRec,								// Child window
			hInst,
			nullptr);												// Pointer not needed

		// Создание поля для вывода результата расшифровки
		CreateWindowW(
			L"EDIT",
			L"",
			WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER,
			rectPane1EditRes.left,
			rectPane1EditRes.top,
			rectPane1EditRes.right,
			rectPane1EditRes.bottom,
			GetDlgItem(hWnd, IDPane1),
			(HMENU)IDPane1EditRes,
			hInst,
			nullptr);

		// Создание контролов для 2-ой панели



		// Создание контролов для 3-ой панели
		CreateWindowW(
			L"EDIT",
			L"",
			WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER,
			rectPane3EditText.left,
			rectPane3EditText.top,
			rectPane3EditText.right,
			rectPane3EditText.bottom,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3EditText,
			hInst,
			nullptr);
		SendMessageW(GetDlgItem(hWnd, IDPane3EditText), EM_LIMITTEXT, MAX_EDITSTRING / 6, 0);

		CreateWindowW(
			L"EDIT",
			L"",
			WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER,
			rectPane3EditCode.left,
			rectPane3EditCode.top,
			rectPane3EditCode.right,
			rectPane3EditCode.bottom,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3EditCode,
			hInst,
			nullptr);
		SendMessageW(GetDlgItem(hWnd, IDPane3EditCode), EM_LIMITTEXT, MAX_EDITSTRING, 0);

		CreateWindow(
			L"BUTTON",
			L"Copy",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			rectPane3ButtCopyText.left,
			rectPane3ButtCopyText.top,
			rectPane3ButtCopyText.right,
			rectPane3ButtCopyText.bottom,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3ButtCopyText,
			hInst,
			nullptr);

		CreateWindow(
			L"BUTTON",
			L"Play",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			rectPane3ButtPlayCode.left,
			rectPane3ButtPlayCode.top,
			rectPane3ButtPlayCode.right,
			rectPane3ButtPlayCode.bottom,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3ButtPlayCode,
			hInst,
			nullptr);

		CreateWindow(
			L"BUTTON",
			L"Copy",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			rectPane3ButtCopyCode.left,
			rectPane3ButtCopyCode.top,
			rectPane3ButtCopyCode.right,
			rectPane3ButtCopyCode.bottom,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3ButtCopyCode,
			hInst,
			nullptr);

		hWndComboBox = CreateWindow(
			L"LISTBOX",
			L"Выпадающий список",
			WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT,
			rectPane3ListDict.left,
			rectPane3ListDict.top,
			rectPane3ListDict.right,
			rectPane3ListDict.bottom,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3ListDict,
			hInst,
			nullptr);

		// Добавление элементов в выпадающий список
		SendMessageW(GetDlgItem(GetDlgItem(hWnd, IDPane3), IDPane3ListDict), LB_ADDSTRING, 0, (LPARAM)L"Dictionary En");
		SendMessageW(GetDlgItem(GetDlgItem(hWnd, IDPane3), IDPane3ListDict), LB_ADDSTRING, 0, (LPARAM)L"Dictionary Ru");
		SendMessageW(GetDlgItem(GetDlgItem(hWnd, IDPane3), IDPane3ListDict), LB_SELECTSTRING, (WPARAM)0, (LPARAM)L"Dictionary En");

		CreateWindow(
			L"BUTTON",
			L"",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_ICON,
			rectPane3ButtAddDic.left,
			rectPane3ButtAddDic.top,
			rectPane3ButtAddDic.right,
			rectPane3ButtAddDic.bottom,
			GetDlgItem(hWnd, IDPane3),
			(HMENU)IDPane3ButtAddDict,
			hInst,
			nullptr);

		// Загрузка иконки
		HICON hIcon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_ICON1));

		// Установка иконки кнопке
		SendMessage(GetDlgItem(GetDlgItem(hWnd, IDPane3), IDPane3ButtAddDict), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

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
	case WM_DRAWITEM:
	{
		// Перерисовываем кнопку
		DrawButton((LPDRAWITEMSTRUCT)lParam, hBrush);
	}
	break;
	case WM_DESTROY:
		DeleteObject(hBrush); // Удаление кисти
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndProcPanes(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rc;

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
	break;
	case CBN_DROPDOWN:
	{
		if (LOWORD(wParam) == IDPane3ListDict)
		{
			// Автоматическое раскрытие выпадающего списка
			SendMessage(hWndComboBox, CB_SHOWDROPDOWN, TRUE, 0);
		}
	}
	break;
	case CBN_SELCHANGE:
	{
		if (LOWORD(wParam) == IDPane3ListDict)
		{

		}
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rc);
		TCHAR szBuffer[MAX_LOADSTRING]; // Буфер для хранения загруженной строки
		int countChar = 0;

		// Установка прозрачного фона для текста
		SetBkMode(hdc, TRANSPARENT);

		// Установка цвета текста
		SetTextColor(hdc, RGB(255, 255, 255)); // Установка красного текста

		if (hWnd == hWndPane1) {
			// Загружаем строку с идентификатором 104 из таблицы строк ресурсов			
			LoadStringW(hInst, 104, szBuffer, MAX_LOADSTRING);
			countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
			TextOutW(hdc, rectPane1EditRes.left, rectPane1EditRes.top - 2 * sizeIndentation.cy, szBuffer, countChar);
		}

		if (hWnd == hWndPane3) {
			LoadStringW(hInst, 105, szBuffer, MAX_LOADSTRING);
			countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
			TextOutW(hdc, rectPane3EditText.left, rectPane3EditText.top - 2 * sizeIndentation.cy, szBuffer, countChar);

			LoadStringW(hInst, 106, szBuffer, MAX_LOADSTRING);
			countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
			TextOutW(hdc, rectPane3EditCode.left, rectPane3EditCode.top - 2 * sizeIndentation.cy, szBuffer, countChar);

			LoadStringW(hInst, 107, szBuffer, MAX_LOADSTRING);
			countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
			TextOutW(hdc, rectPane3ListDict.left, rectPane3ListDict.top - 2 * sizeIndentation.cy, szBuffer, countChar);
		}

		if (hWnd == hWndPane4) {
			HBITMAP hBitmap = (HBITMAP)LoadImage(
				hInst, // Дескриптор экземпляра приложения
				MAKEINTRESOURCE(IDB_BITMAP1), // Идентификатор ресурса изображения
				IMAGE_BITMAP, // Тип изображения
				0, 0, // Ширина и высота загружаемого изображения (0 для автоопределения)
				LR_DEFAULTCOLOR // Флаги загрузки
			);

			// Создание COMPATIBLE DC для рисования
			HDC hdcMem = CreateCompatibleDC(hdc);
			if (hdcMem == NULL) {
				ReleaseDC(hWnd, hdc);
				return NULL;
			}

			// Выбор изображения в COMPATIBLE DC
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

			BITMAP bm;
			GetObject(hBitmap, sizeof(bm), (LPSTR)&bm);

			// Рисование изображения в окне
			BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

			// Очистка ресурсов
			SelectObject(hdcMem, hOldBitmap);
			DeleteDC(hdcMem);

			LoadStringW(hInst, 108, szBuffer, MAX_LOADSTRING);
			countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);

			// Создание нового шрифта
			HFONT hFont = CreateFont(
				40, // Высота шрифта
				0, // Ширина шрифта
				0, // Угол поворота шрифта
				0, // Угол наклона шрифта
				FW_BOLD, // Толщина шрифта
				FALSE, // Курсив
				FALSE, // Подчеркивание
				FALSE, // Зачеркивание
				DEFAULT_CHARSET, // Набор символов
				OUT_DEFAULT_PRECIS, // Точность вывода
				CLIP_DEFAULT_PRECIS, // Точность отсечения
				DEFAULT_QUALITY, // Качество шрифта
				DEFAULT_PITCH | FF_SWISS, // Начертание шрифта
				L"Arial" // Имя шрифта
			);

			// Выбор нового шрифта для контекста устройства
			SelectObject(hdc, hFont);

			TextOutW(hdc, rectPage.right / 2 - 12 * sizeIndentation.cx, rectPage.top + 6 * sizeIndentation.cy, szBuffer, countChar);

			// Создание сплошной кисти с белым цветом
			HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));

			// Заполнение прямоугольника кистью
			RECT rectText = {
				rectPage.right - 450 - 4 * sizeIndentation.cy,
				rectPage.top + 14 * sizeIndentation.cy,
				rectPage.right - 4 * sizeIndentation.cy,
				rectPage.top + 300 + 14 * sizeIndentation.cy
			};
			FillRect(hdc, &rectText, brush);

			// Отображение текста в белом прямоугольнике с помощью DrawText
			hFont = CreateFont(
				16, // Высота шрифта
				0, // Ширина шрифта
				0, // Угол поворота шрифта
				0, // Угол наклона шрифта
				FW_BOLD, // Толщина шрифта
				FALSE, // Курсив
				FALSE, // Подчеркивание
				FALSE, // Зачеркивание
				DEFAULT_CHARSET, // Набор символов
				OUT_DEFAULT_PRECIS, // Точность вывода
				CLIP_DEFAULT_PRECIS, // Точность отсечения
				DEFAULT_QUALITY, // Качество шрифта
				DEFAULT_PITCH | FF_SWISS, // Начертание шрифта
				L"Arial" // Имя шрифта
			);
			// Выбор нового шрифта для контекста устройства
			SelectObject(hdc, hFont);

			SetTextColor(hdc, RGB(0, 0, 0)); // Цвет текста (черный)
			SetBkMode(hdc, TRANSPARENT); // Установка режима прозрачного фона	
			WCHAR text[] = L"Данное приложение разрабатывалось в рамках проекта ВКР по направлению 09.03.01\
				\n\nДанное приложение представляет из себя Аудидекодер Морзе.\
				\nОно способно расшифровывать аудиосигнал с зашифрованным сообщением на азбуке Морзе.";
			rectText = {
				rectPage.right - 450 - 3 * sizeIndentation.cy,
				rectPage.top + 15 * sizeIndentation.cy,
				rectPage.right - 5 * sizeIndentation.cy,
				rectPage.top + 300 + 13 * sizeIndentation.cy
			};
			DrawTextW(hdc, text, -1, &rectText, DT_LEFT | DT_TOP | DT_WORDBREAK);

			// Удаление кисти
			DeleteObject(brush);

			// Освобождение шрифта после использования
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));
			DeleteObject(hFont);
		}

		EndPaint(hWnd, &ps);
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}