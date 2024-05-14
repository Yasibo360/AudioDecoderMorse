#include "MyFunction.h"

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

	HWND hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void OnCreate(HWND hWnd)
{
	// Вывод окна на середину экрана и установка размерв
	MoveWindow(
		hWnd,
		GetSystemMetrics(SM_CXSCREEN) / 2 - settings.windowRect.right / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - settings.windowRect.bottom / 2,
		settings.windowRect.right,
		settings.windowRect.bottom,
		true
	);

	// Создание GUI приложения
	InitializeUI(hWnd);

	HDC hdc = GetDC(hWnd);

	// Создание нового шрифта
	HFONT hFont = CreateFontW(
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
		settings.FontName.c_str()	// Имя шрифта
	);

	// Выбор нового шрифта для контекста устройства
	SelectObject(hdc, hFont);

	DeleteObject(hFont);
	ReleaseDC(hWnd, hdc);

	// Установка локали (для отладки)
	std::setlocale(LC_ALL, "");
}

// Инициализация элементов управления окна, создание кнопок, текстовых полей, и т.д.
void InitializeUI(HWND hWnd)
{
	RECT rc;

	TCHAR szBuffer[MAX_LOADSTRING]; // Буфер для хранения загруженной строки
	int countChar = 0;

	GetClientRect(hWnd, &rc);

	InitControlsRect(rc);

	/*
	  Создание меню навигации
	*/
	CreateWindowW(
		L"STATIC",
		L"",
		WS_VISIBLE | WS_CHILD | SS_OWNERDRAW,
		rc.left,
		rc.top,
		rc.right / gui.menu.menuRatio,
		rc.bottom,
		hWnd,
		(HMENU)IDBgrMenu,
		hInst,
		nullptr);

	CreateWindowW(
		L"BUTTON",
		L"Главная",
		WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
		0,
		gui.menu.rectMenu.bottom / 2 - 2 * gui.menu.sizeMenuButton.cy - 2 * gui.sizeIndentation.cy / 2,
		gui.menu.sizeMenuButton.cx,
		gui.menu.sizeMenuButton.cy,
		hWnd,
		(HMENU)IDButtPane1,
		hInst,
		nullptr);

	CreateWindowW(
		L"BUTTON",
		L"Расширенный режим",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
		0,
		gui.menu.rectMenu.bottom / 2 - gui.menu.sizeMenuButton.cy - gui.sizeIndentation.cy / 2,
		gui.menu.sizeMenuButton.cx,
		gui.menu.sizeMenuButton.cy,
		hWnd,
		(HMENU)IDButtPane2,
		hInst,
		nullptr);

	CreateWindowW(
		L"BUTTON",
		L"Переводчик",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
		0,
		gui.menu.rectMenu.bottom / 2,
		gui.menu.sizeMenuButton.cx,
		gui.menu.sizeMenuButton.cy,
		hWnd,
		(HMENU)IDButtPane3,
		hInst,
		nullptr);

	CreateWindowW(
		L"BUTTON",
		L"О программе",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
		0,
		gui.menu.rectMenu.bottom / 2 + gui.menu.sizeMenuButton.cy + gui.sizeIndentation.cy / 2,
		gui.menu.sizeMenuButton.cx,
		gui.menu.sizeMenuButton.cy,
		hWnd,
		(HMENU)IDButtPane4,
		hInst,
		nullptr);

	/*
	  Создание панелей
	*/
	hWndPane1 = CreateWindow(
		L"STATIC",
		L"",
		WS_VISIBLE | WS_CHILD,
		gui.rectPage.left,
		gui.rectPage.top,
		gui.rectPage.right,
		gui.rectPage.bottom,
		hWnd,
		(HMENU)IDPane1,
		hInst,
		nullptr);
	SetWindowLongPtrW(GetDlgItem(hWnd, IDPane1), GWLP_WNDPROC, (LONG_PTR)WndProc);

	hWndPane2 = CreateWindow(
		L"STATIC",
		L"",
		WS_VISIBLE | WS_CHILD,
		gui.rectPage.left,
		gui.rectPage.top,
		gui.rectPage.right,
		gui.rectPage.bottom,
		hWnd,
		(HMENU)IDPane2,
		hInst,
		nullptr);
	SetWindowLongPtrW(GetDlgItem(hWnd, IDPane2), GWLP_WNDPROC, (LONG_PTR)WndProc);

	hWndPane3 = CreateWindow(
		L"STATIC",
		L"",
		WS_VISIBLE | WS_CHILD,
		gui.rectPage.left,
		gui.rectPage.top,
		gui.rectPage.right,
		gui.rectPage.bottom,
		hWnd,
		(HMENU)IDPane3,
		hInst,
		nullptr);
	SetWindowLongPtrW(GetDlgItem(hWnd, IDPane3), GWLP_WNDPROC, (LONG_PTR)WndProc);

	hWndPane4 = CreateWindow(
		L"STATIC",
		L"",
		WS_VISIBLE | WS_CHILD,
		gui.rectPage.left,
		gui.rectPage.top,
		gui.rectPage.right,
		gui.rectPage.bottom,
		hWnd,
		(HMENU)IDPane4,
		hInst,
		nullptr);
	SetWindowLongPtrW(GetDlgItem(hWnd, IDPane4), GWLP_WNDPROC, (LONG_PTR)WndProc);

	/*
	  Создание контролов для 1-ой панели
	*/

	// Создание кнопки прослушивания
	CreateWindowW(
		szRecButtClass,											// Predefined class; Unicode assumed 
		L"Прослушать",											// Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		gui.pane1.rectPane1ButtonRec.left,						// x position 
		gui.pane1.rectPane1ButtonRec.top,						// y position 
		gui.pane1.rectPane1ButtonRec.right,						// Button width
		gui.pane1.rectPane1ButtonRec.bottom,					// Button height
		GetDlgItem(hWnd, IDPane1),								// Parent window
		(HMENU)IDPane1ButtonRec,								// Child window
		hInst,
		nullptr);												// Pointer not needed
	SetEllipticRegion(GetDlgItem(GetDlgItem(hWnd, IDPane1), IDPane1ButtonRec));

	// Создание поля для вывода результата расшифровки
	CreateWindowW(
		L"EDIT",
		L"",
		WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER,
		gui.pane1.rectPane1EditRes.left,
		gui.pane1.rectPane1EditRes.top,
		gui.pane1.rectPane1EditRes.right,
		gui.pane1.rectPane1EditRes.bottom,
		GetDlgItem(hWnd, IDPane1),
		(HMENU)IDPane1EditRes,
		hInst,
		nullptr);

	// Создание контролов для 2-ой панели
	CreateWindow(
		L"STATIC",
		L"",
		WS_VISIBLE | WS_CHILD,
		gui.pane2.rectPane2Plot.left,
		gui.pane2.rectPane2Plot.top,
		gui.pane2.rectPane2Plot.right,
		gui.pane2.rectPane2Plot.bottom,
		GetDlgItem(hWnd, IDPane2),
		(HMENU)IDPane2Plot,
		hInst,
		nullptr);

	CreateWindow(
		L"BUTTON",
		L"Play",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		gui.pane2.rectPane2ButtPlay.left,
		gui.pane2.rectPane2ButtPlay.top,
		gui.pane2.rectPane2ButtPlay.right,
		gui.pane2.rectPane2ButtPlay.bottom,
		GetDlgItem(hWnd, IDPane2),
		(HMENU)IDPane2ButtPlay,
		hInst,
		nullptr);

	CreateWindow(
		L"BUTTON",
		L"Stop",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		gui.pane2.rectPane2ButtStop.left,
		gui.pane2.rectPane2ButtStop.top,
		gui.pane2.rectPane2ButtStop.right,
		gui.pane2.rectPane2ButtStop.bottom,
		GetDlgItem(hWnd, IDPane2),
		(HMENU)IDPane2ButtStop,
		hInst,
		nullptr);

	CreateWindow(
		L"BUTTON",
		L"Reset",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		gui.pane2.rectPane2ButtReset.left,
		gui.pane2.rectPane2ButtReset.top,
		gui.pane2.rectPane2ButtReset.right,
		gui.pane2.rectPane2ButtReset.bottom,
		GetDlgItem(hWnd, IDPane2),
		(HMENU)IDPane2ButtReset,
		hInst,
		nullptr);

	CreateWindowW(
		L"EDIT",
		L"",
		WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER,
		gui.pane2.rectPane2EditCode.left,
		gui.pane2.rectPane2EditCode.top,
		gui.pane2.rectPane2EditCode.right,
		gui.pane2.rectPane2EditCode.bottom,
		GetDlgItem(hWnd, IDPane2),
		(HMENU)IDPane2EditCode,
		hInst,
		nullptr);
	SendMessageW(GetDlgItem(hWnd, IDPane2EditCode), EM_LIMITTEXT, MAX_EDITSTRING, 0);

	CreateWindowW(
		L"EDIT",
		L"",
		WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER,
		gui.pane2.rectPane2EditText.left,
		gui.pane2.rectPane2EditText.top,
		gui.pane2.rectPane2EditText.right,
		gui.pane2.rectPane2EditText.bottom,
		GetDlgItem(hWnd, IDPane2),
		(HMENU)IDPane2EditText,
		hInst,
		nullptr);
	SendMessageW(GetDlgItem(hWnd, IDPane2EditText), EM_LIMITTEXT, MAX_EDITSTRING, 0);

	// Создание контролов для 3-ой панели
	CreateWindowW(
		L"EDIT",
		L"",
		WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER,
		gui.pane3.rectPane3EditText.left,
		gui.pane3.rectPane3EditText.top,
		gui.pane3.rectPane3EditText.right,
		gui.pane3.rectPane3EditText.bottom,
		GetDlgItem(hWnd, IDPane3),
		(HMENU)IDPane3EditText,
		hInst,
		nullptr);
	SendMessageW(GetDlgItem(hWnd, IDPane3EditText), EM_LIMITTEXT, MAX_EDITSTRING, 0);

	CreateWindowW(
		L"EDIT",
		L"",
		WS_TABSTOP | WS_GROUP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER,
		gui.pane3.rectPane3EditCode.left,
		gui.pane3.rectPane3EditCode.top,
		gui.pane3.rectPane3EditCode.right,
		gui.pane3.rectPane3EditCode.bottom,
		GetDlgItem(hWnd, IDPane3),
		(HMENU)IDPane3EditCode,
		hInst,
		nullptr);
	SendMessageW(GetDlgItem(hWnd, IDPane3EditCode), EM_LIMITTEXT, MAX_EDITSTRING, 0);

	CreateWindow(
		L"BUTTON",
		L"Copy",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		gui.pane3.rectPane3ButtCopyText.left,
		gui.pane3.rectPane3ButtCopyText.top,
		gui.pane3.rectPane3ButtCopyText.right,
		gui.pane3.rectPane3ButtCopyText.bottom,
		GetDlgItem(hWnd, IDPane3),
		(HMENU)IDPane3ButtCopyText,
		hInst,
		nullptr);

	CreateWindow(
		L"BUTTON",
		L"Play",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		gui.pane3.rectPane3ButtPlayCode.left,
		gui.pane3.rectPane3ButtPlayCode.top,
		gui.pane3.rectPane3ButtPlayCode.right,
		gui.pane3.rectPane3ButtPlayCode.bottom,
		GetDlgItem(hWnd, IDPane3),
		(HMENU)IDPane3ButtPlayCode,
		hInst,
		nullptr);

	CreateWindow(
		L"BUTTON",
		L"Copy",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		gui.pane3.rectPane3ButtCopyCode.left,
		gui.pane3.rectPane3ButtCopyCode.top,
		gui.pane3.rectPane3ButtCopyCode.right,
		gui.pane3.rectPane3ButtCopyCode.bottom,
		GetDlgItem(hWnd, IDPane3),
		(HMENU)IDPane3ButtCopyCode,
		hInst,
		nullptr);

	CreateWindow(
		L"LISTBOX",
		L"Выпадающий список",
		WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_NOSEL,
		gui.pane3.rectPane3ListDict.left,
		gui.pane3.rectPane3ListDict.top,
		gui.pane3.rectPane3ListDict.right,
		gui.pane3.rectPane3ListDict.bottom,
		GetDlgItem(hWnd, IDPane3),
		(HMENU)IDPane3ListDict,
		hInst,
		nullptr);

	// Добавление элементов в выпадающий список
	std::vector<std::wstring> dictName = morse.getAllDictionariesName();
	for (const auto& name : dictName)
	{
		SendMessageW(GetDlgItem(GetDlgItem(hWnd, IDPane3), IDPane3ListDict), LB_ADDSTRING, 0, (LPARAM)name.c_str());
	}

	CreateWindow(
		L"BUTTON",
		L"",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_ICON,
		gui.pane3.rectPane3ButtAddDic.left,
		gui.pane3.rectPane3ButtAddDic.top,
		gui.pane3.rectPane3ButtAddDic.right,
		gui.pane3.rectPane3ButtAddDic.bottom,
		GetDlgItem(hWnd, IDPane3),
		(HMENU)IDPane3ButtAddDict,
		hInst,
		nullptr);

	// Загрузка и установка иконки
	HICON hIcon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(GetDlgItem(GetDlgItem(hWnd, IDPane3), IDPane3ButtAddDict), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

	// Создание контролов для 4-ой панели
	CreateWindow(
		L"STATIC",
		L"",
		WS_VISIBLE | WS_CHILD | SS_BITMAP,
		gui.pane4.rectPane4Picture.left,
		gui.pane4.rectPane4Picture.top,
		gui.pane4.rectPane4Picture.right,
		gui.pane4.rectPane4Picture.bottom,
		GetDlgItem(hWnd, IDPane4),
		(HMENU)IDPane4Picture,
		hInst,
		nullptr);

	SelectPane(hWnd, IDPane1);
}

// Инициализация глобальных переменных
void InitControlsRect(RECT rc)
{
	// Инициализация размеров контролов меню
	gui.menu.rectMenu = { rc.left, rc.top, rc.right / gui.menu.menuRatio, rc.bottom };
	gui.menu.sizeMenuButton = { gui.menu.rectMenu.right, 50 };

	// Инициализация размера страниц
	gui.rectPage = { gui.menu.rectMenu.right, rc.top, rc.right - gui.menu.rectMenu.right, rc.bottom };

	// Инициализация размеров контролов для главной страницы
	gui.pane1.rectPane1ButtonRec =
	{
		(gui.rectPage.right - 180) / 2,
		(gui.rectPage.bottom - 180) / 2,
		180,
		180
	};
	gui.pane1.rectPane1EditRes = {
		(gui.rectPage.right - 600) / 2,
		(gui.rectPage.bottom - 200) - 2 * gui.sizeIndentation.cy,
		600,
		200
	};

	// Инициализация размеров контролов для страницы редактирования
	gui.pane2.rectPane2ButtPlay = {
		gui.rectPage.right / 2 - gui.sizeButton.cx / 2,
		gui.rectPage.top + 2 * gui.sizeIndentation.cy,
		gui.sizeButton.cx,
		gui.sizeButton.cy,
	};
	gui.pane2.rectPane2ButtStop = {
		gui.rectPage.right / 2 - gui.sizeButton.cx - gui.sizeButton.cx / 2 - gui.sizeIndentation.cx,
		gui.rectPage.top + 2 * gui.sizeIndentation.cy,
		gui.sizeButton.cx,
		gui.sizeButton.cy,
	};
	gui.pane2.rectPane2ButtReset = {
		gui.rectPage.right / 2 + gui.sizeButton.cx / 2 + gui.sizeIndentation.cx,
		gui.rectPage.top + 2 * gui.sizeIndentation.cy,
		gui.sizeButton.cx,
		gui.sizeButton.cy,
	};
	gui.pane2.rectPane2Plot = {
		gui.sizeIndentation.cx,
		gui.pane2.rectPane2ButtPlay.top + gui.pane2.rectPane2ButtPlay.bottom + gui.sizeIndentation.cy,
		gui.rectPage.right - 2 * gui.sizeIndentation.cx,
		gui.rectPage.bottom / 2 + 6 * gui.sizeIndentation.cy
	};
	gui.pane2.rectPane2EditCode = {
		gui.pane2.rectPane2Plot.left,
		gui.pane2.rectPane2Plot.top + gui.pane2.rectPane2Plot.bottom + 4 * gui.sizeIndentation.cx,
		gui.pane2.rectPane2Plot.right / 2 - gui.sizeIndentation.cx,
		gui.rectPage.bottom - (gui.pane2.rectPane2Plot.top + gui.pane2.rectPane2Plot.bottom + 4 * gui.sizeIndentation.cx) - gui.sizeIndentation.cy
	};
	gui.pane2.rectPane2EditText = {
		gui.pane2.rectPane2Plot.right / 2 + gui.sizeIndentation.cx,
		gui.pane2.rectPane2Plot.top + gui.pane2.rectPane2Plot.bottom + 4 * gui.sizeIndentation.cx,
		gui.pane2.rectPane2Plot.right / 2,
		gui.rectPage.bottom - (gui.pane2.rectPane2Plot.top + gui.pane2.rectPane2Plot.bottom + 4 * gui.sizeIndentation.cx) - gui.sizeIndentation.cy
	};
	
	// Инициализация размеров контролов для страницы переводчика
	gui.pane3.rectPane3EditText =
	{
		gui.rectPage.right / 2 - 300 - 2 * gui.sizeIndentation.cx,
		gui.rectPage.top + gui.pane3.indOnTopPane3 * gui.sizeIndentation.cy,
		300,
		300
	};
	gui.pane3.rectPane3EditCode =
	{
		gui.rectPage.right / 2 + 2 * gui.sizeIndentation.cx,
		gui.rectPage.top + gui.pane3.indOnTopPane3 * gui.sizeIndentation.cy,
		300,
		300
	};
	gui.pane3.rectPane3ButtCopyText =
	{
		gui.pane3.rectPane3EditText.left + gui.pane3.rectPane3EditText.right - gui.sizeButton.cx,
		gui.pane3.rectPane3EditText.top + gui.pane3.rectPane3EditText.bottom + gui.sizeIndentation.cy,
		gui.sizeButton.cx,
		gui.sizeButton.cy
	};
	gui.pane3.rectPane3ButtPlayCode =
	{
		gui.pane3.rectPane3EditCode.left + gui.pane3.rectPane3EditCode.right - 2 * gui.sizeButton.cx - gui.sizeIndentation.cx,
		gui.pane3.rectPane3EditCode.top + gui.pane3.rectPane3EditCode.bottom + gui.sizeIndentation.cy,
		gui.sizeButton.cx,
		gui.sizeButton.cy
	};
	gui.pane3.rectPane3ButtCopyCode =
	{
		gui.pane3.rectPane3EditCode.left + gui.pane3.rectPane3EditCode.right - gui.sizeButton.cx,
		gui.pane3.rectPane3EditCode.top + gui.pane3.rectPane3EditCode.bottom + gui.sizeIndentation.cy,
		gui.sizeButton.cx,
		gui.sizeButton.cy
	};
	gui.pane3.rectPane3ListDict =
	{
		gui.rectPage.right / 2 - 3 * gui.sizeButton.cx,
		gui.pane3.rectPane3ButtPlayCode.top + gui.pane3.rectPane3ButtPlayCode.bottom + 5 * gui.sizeIndentation.cy,
		6 * gui.sizeButton.cx,
		4 * gui.sizeButton.cy
	};
	gui.pane3.rectPane3ButtAddDic =
	{
		gui.pane3.rectPane3ListDict.left + gui.pane3.rectPane3ListDict.right + gui.sizeIndentation.cx,
		gui.pane3.rectPane3ButtPlayCode.top + gui.pane3.rectPane3ButtPlayCode.bottom + 5 * gui.sizeIndentation.cx,
		gui.sizeButton.cx,
		gui.sizeButton.cy
	};

	// Инициализация размеров контролов для страницы описания
	gui.pane4.rectPane4Picture =
	{
		gui.rectPage.left,
		gui.rectPage.top,
		gui.rectPage.right,
		gui.rectPage.bottom
	};

	return;
}

void SetEllipticRegion(HWND hWnd)
{
	HRGN hRgn{};
	RECT rc;

	GetWindowRect(hWnd, &rc);

	// Смещаем прямоугольник, чтобы верхний левый угол был (0, 0)
	OffsetRect(&rc, -rc.left, -rc.top);

	// Создаем регион в виде эллипса, ограниченного прямоугольником rc
	hRgn = CreateEllipticRgnIndirect(&rc);

	// Устанавливаем созданный регион для кнопки и производим немедленную перерисовку
	SetWindowRgn(hWnd, hRgn, TRUE);
}

// =====================================
// Функция DrawButton
// Перерисовывает кнопку
// =====================================
void DrawButton(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo)
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

	HBRUSH hBrush = CreateSolidBrush(settings.windowColorBckgdMenuButt);

	FillRect(lpInfo->hDC, &lpInfo->rcItem, hBrush);

	// Если кнопка выбрана, рисуем ее в нажатом
	// состоянии
	if (lpInfo->itemState & ODS_SELECTED)
	{
		HBRUSH hBrushPressed = CreateSolidBrush(settings.windowColorBckgdMenuButtPressed);
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
	DeleteObject(hBrush); // Удаление кисти
}

void DrawStatic(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo)
{
	HDC hdc = lpInfo->hDC;
	RECT rect = lpInfo->rcItem;

	// Установить цвет фона
	HBRUSH hBrush = CreateSolidBrush(settings.windowColorBckgdMenu);
	FillRect(hdc, &rect, hBrush);
	DeleteObject(hBrush);
}

void SelectPane(HWND& hWnd, int ID)
{
	for (size_t i = 0; i < IDPanes.size(); i++)
	{
		if (IDPanes[i] == ID) {
			ShowWindow(GetDlgItem(hWnd, IDPanes[i]), SW_SHOW);
		}
		else {
			ShowWindow(GetDlgItem(hWnd, IDPanes[i]), SW_HIDE);
		}
	}
}

void DrawPane1(HWND& hWnd, HINSTANCE& hInst, HDC& hdc)
{
	int countChar = 0;
	TCHAR szBuffer[MAX_LOADSTRING]; // Буфер для хранения загруженной строки

	// Загружаем строку с идентификатором IDS_STRING104 из таблицы строк ресурсов			
	LoadStringW(hInst, IDS_STRING104, szBuffer, MAX_LOADSTRING);
	countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
	TextOutW(hdc, gui.pane1.rectPane1EditRes.left, gui.pane1.rectPane1EditRes.top - 2 * gui.sizeIndentation.cy, szBuffer, countChar);
}

void DrawPane2(HWND& hWnd, HINSTANCE& hInst, HDC& hdc)
{
	int countChar = 0;
	TCHAR szBuffer[MAX_LOADSTRING]; // Буфер для хранения загруженной строки

	LoadStringW(hInst, IDS_STRING105, szBuffer, MAX_LOADSTRING);
	countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
	TextOutW(hdc, gui.pane2.rectPane2EditText.left, gui.pane2.rectPane2EditText.top - 2 * gui.sizeIndentation.cy, szBuffer, countChar);

	LoadStringW(hInst, IDS_STRING106, szBuffer, MAX_LOADSTRING);
	countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
	TextOutW(hdc, gui.pane2.rectPane2EditCode.left, gui.pane2.rectPane2EditCode.top - 2 * gui.sizeIndentation.cy, szBuffer, countChar);
}

void DrawPane3(HWND& hWnd, HINSTANCE& hInst, HDC& hdc)
{
	int countChar = 0;
	TCHAR szBuffer[MAX_LOADSTRING]; // Буфер для хранения загруженной строки

	LoadStringW(hInst, IDS_STRING105, szBuffer, MAX_LOADSTRING);
	countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
	TextOutW(hdc, gui.pane3.rectPane3EditText.left, gui.pane3.rectPane3EditText.top - 2 * gui.sizeIndentation.cy, szBuffer, countChar);

	LoadStringW(hInst, IDS_STRING106, szBuffer, MAX_LOADSTRING);
	countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
	TextOutW(hdc, gui.pane3.rectPane3EditCode.left, gui.pane3.rectPane3EditCode.top - 2 * gui.sizeIndentation.cy, szBuffer, countChar);

	LoadStringW(hInst, IDS_STRING107, szBuffer, MAX_LOADSTRING);
	countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
	TextOutW(hdc, gui.pane3.rectPane3ListDict.left, gui.pane3.rectPane3ListDict.top - 2 * gui.sizeIndentation.cy, szBuffer, countChar);
}

void DrawPane4(HWND& hWnd, HINSTANCE& hInst, HDC& hdc)
{
	int countChar = 0;
	TCHAR szBuffer[MAX_LOADSTRING]; // Буфер для хранения загруженной строки

	DrawImage(hWnd, hInst, hdc, IDB_BITMAP1);

	LoadStringW(hInst, IDS_STRING108, szBuffer, MAX_LOADSTRING);
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
	HGDIOBJ oldFont = SelectObject(hdc, hFont);

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

	// Выбор нового шрифта для контекста устройства
	SelectObject(hdc, oldFont);

	SetTextColor(hdc, RGB(0, 0, 0));
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
	DeleteObject(hFont);
}

void DrawPlot(HWND& hWnd, sf::Texture& texture)
{
	if (texture.getSize().x != 0 || texture.getSize().y != 0) {
		// Создаем экземпляр sf::RenderWindow с контекстом устройства окна
		sf::RenderWindow window(GetDlgItem(hWnd, IDPane2Plot));

		// Создаем объект sf::Sprite и устанавливаем на него текстуру
		sf::Sprite sprite;
		sprite.setTexture(texture);

		// Отображаем текстуру в окне
		window.clear();
		window.draw(sprite);
		window.display();

		UpdateWindow(GetDlgItem(hWnd, IDPane2Plot));
	}
}

void processPlotData(const std::string& fileName, const std::string& curveName, const int typeLine, const sf::Color& color, HWND hWnd, int controlID, Plot_AmpTime& plot)
{
    if (plot.getCurveCount() != 0 && plot.haveCurve(curveName.c_str()) == false)
    {
        SndfileHandle file(fileName);

        if (file.error() != 0) {
            OutputDebugString(L"Ошибка при открытии файла: ");
            OutputDebugString(std::to_wstring(sf_error(file.rawHandle())).c_str());
        }

        std::vector<std::vector<float>> samplesByChannel;
        sf_count_t frames_t;
        frames_t = readAudioData(file, samplesByChannel);

        if (frames_t != 0) {
            float samplerate = file.samplerate();
            float frames = frames_t / file.channels();
            std::vector<float> time(frames);

            for (int i = 0; i < frames; i++) {
                time[i] = i / samplerate;
            }

			auto boundFunction = std::bind(&Plot_AmpTime::graphThresholdLine, &plot, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			switch (typeLine)
			{
			case 0:
			{
				boundFunction = std::bind(&Plot_AmpTime::graphThresholdLine, &plot, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			}
			break;
			case 1:
			{
				boundFunction = std::bind(&Plot_AmpTime::graphMeanLine, &plot, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			}
			break;
			case 2:
			{
				boundFunction = std::bind(&Plot_AmpTime::graphMeanPosLine, &plot, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			}
			break;
			case 3:
			{
				boundFunction = std::bind(&Plot_AmpTime::graphMeanNegLine, &plot, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			}
			break;
			default:
				break;
			}

            plot.addCurve(curveName.c_str(), boundFunction, time, samplesByChannel[0], color, 2);

            sf::RenderWindow window(GetDlgItem(hWnd, controlID));
            window.draw(plot);
            window.display();
        }
    }
}

void createPopupMenuPlot(HWND hWnd, POINT pt)
{
	HMENU hSubMenu = CreatePopupMenu();
	AppendMenu(hSubMenu, MF_STRING | MF_UNCHECKED, IDMenuItem3, L"Общее");
	AppendMenu(hSubMenu, MF_STRING | MF_UNCHECKED, IDMenuItem4, L"Для положительньных значений");
	AppendMenu(hSubMenu, MF_STRING | MF_UNCHECKED, IDMenuItem5, L"Для отрицательных значений");

	HMENU hContextMenu = CreatePopupMenu();
	AppendMenu(hContextMenu, MF_STRING, IDMenuItem1, L"Открыть файл");
	AppendMenu(hContextMenu, MF_STRING | MF_UNCHECKED, IDMenuItem2, L"Пороговое значение");
	InsertMenu(hContextMenu, -1, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hSubMenu, L"Среднее значение");
	TrackPopupMenu(hContextMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);

}

sf_count_t readAudioData(SndfileHandle file, std::vector<std::vector<float>>& samplesByChannel)
{
	// Проверка, удалось ли открыть файл для чтения
	if (!file) {
		std::cerr << "Не удалось открыть файл для чтения" << std::endl;
		throw "Не удалось открыть файл для чтения";
	}

	// Получаем параметры аудиофайла
	int frames = file.frames();
	int channels = file.channels();
	int samplerate = file.samplerate();

	// Чтение данных из файла
	std::vector<float> audioData(frames * channels);
	sf_count_t framesRead = file.read(audioData.data(), audioData.size());

	// Проверка, удалось ли прочитать данные из файла
	if (framesRead != audioData.size()) {
		std::cerr << "Не удалось прочитать данные из файла" << std::endl;
		throw "Не удалось прочитать данные из файла";
	}

	// Вектор векторов для чтения по каналам
	samplesByChannel.resize(channels);

	for (int i = 0; i < frames; ++i) {
		for (int j = 0; j < channels; ++j) {
			samplesByChannel[j].push_back(audioData[i * channels + j]);
		}
	}

	return framesRead;
}

void addDictionary(const HWND hWnd, const int IDControl)
{
	std::string fileName = openFileDialog();
	if (morse.loadDictionaryFromFile(fileName)) {
		// Очищаем лист словарей
		SendMessageW(GetDlgItem(hWnd, IDPane3ListDict), LB_RESETCONTENT, 0, 0);

		// Заполняем лист словарей
		std::vector<std::wstring> dictName = morse.getAllDictionariesName();
		for (const auto& name : dictName)
		{
			SendMessageW(GetDlgItem(hWnd, IDPane3ListDict), LB_ADDSTRING, 0, (LPARAM)name.c_str());
		}
	}
}

std::string openFileDialog()
{
	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAMEW));

	wchar_t fileName[MAX_PATH + 1];
	ZeroMemory(fileName, sizeof(fileName));

	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.hwndOwner = NULL; // Указывается только в случае использования внутри окна
	ofn.lpstrFilter = L"All Files\0*.*\0Wave Files\0*.wav\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	std::wstring filePath;
	if (GetOpenFileNameW(&ofn) == TRUE)
	{
		filePath = ofn.lpstrFile;
	}

	std::string filePathStr(filePath.begin(), filePath.end());

	return filePathStr;
}

void RecordWithDecode(const HWND hWnd, const int IDControl)
{
	if (!recorder.IsRecording()) {
		recorder.StartRecording();
	}
	else {
		recorder.StopRecording();

		std::wstring morseCode;
		std::wstring morseChar;
		std::vector<std::pair<float, float>> widePeaks;
		std::vector<std::pair<char, float>> peakDurations;

		widePeaks = morse.findWidePeaksInAudioFile("recorded.wav");
		peakDurations = morse.findPeakDurations(widePeaks, 2);
		morseCode = morse.peakDurationsToMorse(peakDurations);
		morseChar = morse.morseToChar(morseCode);

		WCHAR buffer[MAX_EDITSTRING];
		if (morseChar.length() != 0)
		{
			for (size_t i = 0; i < morseChar.length(); i++)
			{
				buffer[i] = morseChar[i];
			}
			buffer[morseChar.length()] = '\0';
			SetWindowTextW(GetDlgItem(hWnd, IDControl), buffer);
		}
		else
		{
			SetWindowTextW(GetDlgItem(hWnd, IDControl), 0);
		}
	}

	//morse.audioFileToMorse("recorded.wav");

	//std::wstring morseCode;
	//std::wstring morseChar;
	//morseCode = morse.audioFileToMorse("recorded.wav");
	//morseChar = morse.morseToChar(morseCode);

	//WCHAR buffer[MAX_EDITSTRING];
	//if (morseChar.length() != 0)
	//{
	//	for (size_t i = 0; i < morseChar.length(); i++)
	//	{
	//		buffer[i] = morseChar[i];
	//		buffer[i + 1] = '\0';
	//	}
	//	SetWindowTextW(GetDlgItem(hWndPane1, IDPane1EditRes), buffer);
	//}
	//else
	//{
	//	SetWindowTextW(GetDlgItem(hWnd, IDPane1EditRes), 0);
	//}
}

void DrawImage(HWND& hWnd, HINSTANCE& hInst, HDC& hdc, int IDB_BITMAP)
{
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
		return;
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
}

void PlayCodeMorse(HWND& hWnd)
{
	WCHAR buffer[MAX_EDITSTRING];
	std::wstring morseCode;

	GetWindowTextW(GetDlgItem(hWnd, IDPane3EditCode), buffer, MAX_LOADSTRING);
	for (size_t i = 0; (i < sizeof(buffer) / 2 - 1) && (buffer[i] != 0); i++)
	{
		morseCode += buffer[i];
	}

	if (!morseCode.empty()) {
		std::thread thread([morseCode = std::move(morseCode)]() mutable {
			morse.playMorseCode(morseCode);
			});
		thread.detach();
	}
}

void ChangeEditText(HWND& hWnd)
{
	WCHAR buffer[MAX_EDITSTRING];
	std::wstring morseChar;
	std::wstring morseCode;

	GetWindowTextW(GetDlgItem(hWnd, IDPane3EditText), buffer, MAX_EDITSTRING);
	for (size_t i = 0; (i < sizeof(buffer) / 2 - 1) && (buffer[i] != 0); i++)
	{
		morseChar += buffer[i];
	}
	morseCode = morse.charToMorse(morseChar);
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

std::vector<std::wstring> Split(const std::wstring& input, const std::wstring& delimiter)
{
	std::vector<std::wstring> result;
	size_t start = 0;
	size_t end = input.find(delimiter);

	while (end != std::wstring::npos) {
		result.push_back(input.substr(start, end - start));
		start = end + delimiter.length();
		end = input.find(delimiter, start);
	}

	result.push_back(input.substr(start));

	return result;
}

void CopyIntoBuffer(const HWND hWnd, const int IDControl)
{
	double textLength = GetWindowTextLength(GetDlgItem(hWnd, IDControl));
	if (textLength > 0)
	{
		WCHAR* buffer = new WCHAR[textLength + 1];
		GetWindowText(GetDlgItem(hWnd, IDControl), buffer, textLength + 1);

		// Копирование текста в буфер обмена
		if (OpenClipboard(hWnd))
		{
			EmptyClipboard();
			HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (textLength + 1) * sizeof(WCHAR));
			if (hGlobal)
			{
				LPTSTR clipboardData = (LPTSTR)GlobalLock(hGlobal);
				lstrcpy(clipboardData, buffer);
				GlobalUnlock(hGlobal);
				SetClipboardData(CF_UNICODETEXT, hGlobal);
			}
			CloseClipboard();
		}
		delete[] buffer;
	}
}

int CountCharactersToEnd(TCHAR* szBuffer, int maxLength)
{
	int count = 0;

	for (int i = 0; i < maxLength; i++)
	{
		count++;
		if (szBuffer[i] == _T('\0'))
		{
			break;
		}
	}

	return count;
}
