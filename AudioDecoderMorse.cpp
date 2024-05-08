// Decoder Morze.cpp : Определяет точку входа для приложения.
//

#include "GlobalVar.h"
#include "MyFunction.h"

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
		OnCreate(hWnd, hBrush);
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
				SelectPage(hWnd, IDPane1);
			}
			break;
			case IDButtPane2:
			{
				SelectPage(hWnd, IDPane2);
			}
			break;
			case IDButtPane3:
			{
				SelectPage(hWnd, IDPane3);
			}
			break;
			case IDButtPane4:
			{
				SelectPage(hWnd, IDPane4);
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
		DrawButton(hInst, (LPDRAWITEMSTRUCT)lParam, hBrush);
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
			SendMessage(GetDlgItem(GetDlgItem(hWnd, IDPane3), IDPane3ListDict), CB_SHOWDROPDOWN, TRUE, 0);
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

		if (hWnd == GetDlgItem(hWnd, IDPane1)) {
			// Загружаем строку с идентификатором 104 из таблицы строк ресурсов			
			LoadStringW(hInst, 104, szBuffer, MAX_LOADSTRING);
			countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
			TextOutW(hdc, gui.pane1.rectPane1EditRes.left, gui.pane1.rectPane1EditRes.top - 2 * gui.sizeIndentation.cy, szBuffer, countChar);
		}

		if (hWnd == GetDlgItem(hWnd, IDPane3)) {
			LoadStringW(hInst, 105, szBuffer, MAX_LOADSTRING);
			countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
			TextOutW(hdc, gui.pane3.rectPane3EditText.left, gui.pane3.rectPane3EditText.top - 2 * gui.sizeIndentation.cy, szBuffer, countChar);

			LoadStringW(hInst, 106, szBuffer, MAX_LOADSTRING);
			countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
			TextOutW(hdc, gui.pane3.rectPane3EditCode.left, gui.pane3.rectPane3EditCode.top - 2 * gui.sizeIndentation.cy, szBuffer, countChar);

			LoadStringW(hInst, 107, szBuffer, MAX_LOADSTRING);
			countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
			TextOutW(hdc, gui.pane3.rectPane3ListDict.left, gui.pane3.rectPane3ListDict.top - 2 * gui.sizeIndentation.cy, szBuffer, countChar);
		}

		if (hWnd == GetDlgItem(hWnd, IDPane4)) {
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

			TextOutW(
				hdc, 
				gui.rectPage.right / 2 - 12 * gui.sizeIndentation.cx, 
				gui.rectPage.top + 6 * gui.sizeIndentation.cy, 
				szBuffer, 
				countChar
			);

			// Создание сплошной кисти с белым цветом
			HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));

			// Заполнение прямоугольника кистью
			RECT rectText = {
				gui.rectPage.right - 450 - 4 * gui.sizeIndentation.cy,
				gui.rectPage.top + 14 * gui.sizeIndentation.cy,
				gui.rectPage.right - 4 * gui.sizeIndentation.cy,
				gui.rectPage.top + 300 + 14 * gui.sizeIndentation.cy
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
				gui.rectPage.right - 450 - 3 * gui.sizeIndentation.cy,
				gui.rectPage.top + 15 * gui.sizeIndentation.cy,
				gui.rectPage.right - 5 * gui.sizeIndentation.cy,
				gui.rectPage.top + 300 + 13 * gui.sizeIndentation.cy
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