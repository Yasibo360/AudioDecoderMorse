// Decoder Morze.cpp : Определяет точку входа для приложения.
//

#include "GlobalVar.h"
#include "MyFunction.h"

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
	LoadStringW(hInstance, IDC_AUDIORECODER, szRecButtClass, MAX_LOADSTRING);

	// Считываем настройки программы из файла настроек
	settings.LoadSettings();

	// Регистрация классов
	MyRegisterClass(hInstance, WndProc, CreateSolidBrush(settings.windowColorBckgd), szWindowClass);
	MyRegisterClass(hInstance, ButtonProc, CreateSolidBrush(settings.windowColorBckgd), szRecButtClass);

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

	switch (message)
	{
	case WM_CREATE:
	{
		OnCreate(hWnd);
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
				SelectPane(hWnd, IDPane1);
			}
			break;
			case IDButtPane2:
			{
				SelectPane(hWnd, IDPane2);
			}
			break;
			case IDButtPane3:
			{
				SelectPane(hWnd, IDPane3);
			}
			break;
			case IDButtPane4:
			{
				SelectPane(hWnd, IDPane4);
			}
			break;
			case IDPane3ButtCopyText:
			{
				CopyIntoBuffer(hWnd, IDPane3EditText);
			}
			break;
			case IDPane3ButtPlayCode:
			{
				PlayCodeMorse(hWnd);
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
		break;
		case EN_CHANGE:
		{
			switch LOWORD(wParam)
			{
			case IDPane3EditText:
			{
				if (GetWindowTextLength(GetDlgItem(hWnd, IDPane3EditText)) < SendMessageW(GetDlgItem(hWnd, IDPane3EditText), EM_GETLIMITTEXT, 0, 0) - 10)
				{
					ChangeEditText(hWnd);
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
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case CBN_DROPDOWN:
	{
		if (LOWORD(wParam) == IDPane3ListDict)
		{
			// Автоматическое раскрытие выпадающего списка		
			SendMessage(GetDlgItem(GetDlgItem(hWnd, IDPane3), IDPane3ListDict), CB_SHOWDROPDOWN, TRUE, 0);
		}
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rc);

		// Установка прозрачного фона для текста
		SetBkMode(hdc, TRANSPARENT);

		// Установка цвета текста
		SetTextColor(hdc, RGB(255, 255, 255)); // Установка цвета текста

		// Создание нового шрифта
		HFONT hFont = CreateFont(
			16,			// Высота шрифта
			0,			// Ширина шрифта
			0,			// Угол поворота шрифта
			0,			// Угол наклона шрифта
			FW_BOLD,	// Толщина шрифта
			FALSE,		// Курсив
			FALSE,		// Подчеркивание
			FALSE,		// Зачеркивание
			DEFAULT_CHARSET,			// Набор символов
			OUT_DEFAULT_PRECIS,			// Точность вывода
			CLIP_DEFAULT_PRECIS,		// Точность отсечения
			DEFAULT_QUALITY,			// Качество шрифта
			DEFAULT_PITCH | FF_SWISS,	// Начертание шрифта
			L"Arial"	// Имя шрифта
		);

		// Выбор нового шрифта для контекста устройства
		SelectObject(hdc, hFont);

		if (hWnd == GetParent(GetDlgItem(hWnd, IDPane1ButtonRec))) {
			DrawPane1(hWnd, hInst, hdc);
		}

		if (hWnd == GetParent(GetDlgItem(hWnd, IDPane3EditText))) {
			DrawPane3(hWnd, hInst, hdc);
		}

		if (hWnd == GetParent(GetDlgItem(hWnd, IDPane4Picture))) {
			DrawPane4(hWnd, hInst, hdc);
		}

		// Освобождение шрифта после использования
		SelectObject(hdc, GetStockObject(SYSTEM_FONT));
		DeleteObject(hFont);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;

		switch (lpDrawItem->CtlID)
		{
		case IDBgrMenu:
		{
			DrawStatic(hInst, lpDrawItem);
		}
		break;
		default:
			DrawButton(hInst, lpDrawItem);
		}
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

// Обработчик рисования кнопки
LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rc;

	switch (message)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rc);

		// Создание нового шрифта
		HFONT hFont = CreateFont(
			20,			// Высота шрифта
			0,			// Ширина шрифта
			0,			// Угол поворота шрифта
			0,			// Угол наклона шрифта
			FW_DEMIBOLD,	// Толщина шрифта
			FALSE,		// Курсив
			FALSE,		// Подчеркивание
			FALSE,		// Зачеркивание
			DEFAULT_CHARSET,			// Набор символов
			OUT_DEFAULT_PRECIS,			// Точность вывода
			CLIP_DEFAULT_PRECIS,		// Точность отсечения
			DEFAULT_QUALITY,			// Качество шрифта
			DEFAULT_PITCH | FF_SWISS,	// Начертание шрифта
			L"Arial"	// Имя шрифта
		);

		// Выбор нового шрифта для контекста устройства
		SelectObject(hdc, hFont);

		// Рисуем фон кнопки
		HBRUSH hBrush = CreateSolidBrush(settings.windowColorBckgdRecButton);
		HRGN hRegion = CreateEllipticRgn(0, 0, 0, 0);

		GetWindowRgn(hWnd, hRegion);
		FillRgn(hdc, hRegion, hBrush);

		// Рисуем рамку эллипса другим цветом
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Создаем красную пенсиль
		hBrush = CreateSolidBrush(settings.windowColorBckgdRecButton2);

		SelectObject(hdc, hPen); // Выбираем пенсиль для контура
		FrameRgn(hdc, hRegion, hBrush, 5, 5); // Рисуем рамку эллипса

		// Рисуем текст кнопки
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));

		WCHAR nameButton[MAX_LOADSTRING];
		LoadStringW(hInst, IDS_STRING115, nameButton, MAX_LOADSTRING);

		DrawTextW(hdc, nameButton, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// Освобождаем ресурсы
		DeleteObject(hPen);
		DeleteObject(hRegion);
		DeleteObject(hBrush);
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		// Получаем контекст устройства кнопки
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &rc);

		// Создание нового шрифта
		HFONT hFont = CreateFont(
			20,			// Высота шрифта
			0,			// Ширина шрифта
			0,			// Угол поворота шрифта
			0,			// Угол наклона шрифта
			FW_BOLD,	// Толщина шрифта
			FALSE,		// Курсив
			TRUE,		// Подчеркивание
			FALSE,		// Зачеркивание
			DEFAULT_CHARSET,			// Набор символов
			OUT_DEFAULT_PRECIS,			// Точность вывода
			CLIP_DEFAULT_PRECIS,		// Точность отсечения
			DEFAULT_QUALITY,			// Качество шрифта
			DEFAULT_PITCH | FF_SWISS,	// Начертание шрифта
			L"Arial"	// Имя шрифта
		);

		// Выбор нового шрифта для контекста устройства
		SelectObject(hdc, hFont);

		HBRUSH hBrush = CreateSolidBrush(settings.windowColorBckgdRecButton2);
		HRGN hRegion = CreateEllipticRgn(0, 0, 0, 0);

		GetWindowRgn(hWnd, hRegion);
		FillRgn(hdc, hRegion, hBrush);

		// Рисуем текст кнопки
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));

		WCHAR nameButton[MAX_LOADSTRING];
		LoadStringW(hInst, IDS_STRING115, nameButton, MAX_LOADSTRING);

		DrawText(hdc, nameButton, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// Освобождаем контекст устройства кнопки
		ReleaseDC(hWnd, hdc);
		DeleteObject(hRegion);
		DeleteObject(hBrush);
		DeleteObject(hFont);
	}
	break;
	case WM_LBUTTONUP:
	{
		// Перерисовываем кнопку
		InvalidateRect(hWnd, nullptr, FALSE);

		RecordWithDecode();
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}