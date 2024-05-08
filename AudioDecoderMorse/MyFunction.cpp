#include "MyFunction.h"

void OnCreate(HWND hWnd, HBRUSH hBrush)
{
	// Считываем настройки программы из файла настроек
	settings.LoadSettings();

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
	InitializeUI(hWnd, hBrush);

	// Установка локали (для отладки)
	std::setlocale(LC_ALL, "");	
}

// Инициализация элементов управления окна, создание кнопок, текстовых полей, и т.д.
void InitializeUI(HWND hWnd, HBRUSH hBrush)
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
		WS_VISIBLE | WS_CHILD | SS_SIMPLE,
		rc.left,
		rc.top,
		rc.right / gui.menu.menuRatio,
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
		gui.menu.rectMenu.bottom / 2 - 2 * gui.menu.sizeMenuButton.cy,
		gui.menu.sizeMenuButton.cx,
		gui.menu.sizeMenuButton.cy,
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
		gui.menu.rectMenu.bottom / 2 - gui.menu.sizeMenuButton.cy,
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
		gui.menu.rectMenu.bottom / 2 + gui.menu.sizeMenuButton.cy,
		gui.menu.sizeMenuButton.cx,
		gui.menu.sizeMenuButton.cy,
		hWnd,
		(HMENU)IDButtPane4,
		hInst,
		nullptr);

	/*
	  Создание панелей
	*/
	CreateWindow(
		szWndClassPane,
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

	CreateWindow(
		szWndClassPane,
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
	ShowWindow(GetDlgItem(hWnd, IDPane2), SW_HIDE);

	CreateWindow(
		szWndClassPane,
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
	ShowWindow(GetDlgItem(hWnd, IDPane3), SW_HIDE);

	CreateWindow(
		szWndClassPane,
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
	ShowWindow(GetDlgItem(hWnd, IDPane4), SW_HIDE);

	/*
	  Создание контролов для 1-ой панели
	*/

	// Создание кнопки прослушивания
	CreateWindowW(
		L"BUTTON",												// Predefined class; Unicode assumed 
		L"Прослушать",											// Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		gui.pane1.rectPane1ButtonRec.left,							// x position 
		gui.pane1.rectPane1ButtonRec.top,								// y position 
		gui.pane1.rectPane1ButtonRec.right,							// Button width
		gui.pane1.rectPane1ButtonRec.bottom,							// Button height
		GetDlgItem(hWnd, IDPane1),								// Parent window
		(HMENU)IDPane1ButtonRec,								// Child window
		hInst,
		nullptr);												// Pointer not needed

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
	SendMessageW(GetDlgItem(hWnd, IDPane3EditText), EM_LIMITTEXT, MAX_EDITSTRING / 6, 0);

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
		WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT,
		gui.pane3.rectPane3ListDict.left,
		gui.pane3.rectPane3ListDict.top,
		gui.pane3.rectPane3ListDict.right,
		gui.pane3.rectPane3ListDict.bottom,
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
		gui.pane3.rectPane3ButtAddDic.left,
		gui.pane3.rectPane3ButtAddDic.top,
		gui.pane3.rectPane3ButtAddDic.right,
		gui.pane3.rectPane3ButtAddDic.bottom,
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
		(gui.rectPage.right - 150) / 2,
		(gui.rectPage.bottom - 150) / 2,
		150,
		150
	};
	gui.pane1.rectPane1EditRes = {
		(gui.rectPage.right - 600) / 2,
		(gui.rectPage.bottom - 200) - 2 * gui.sizeIndentation.cy,
		600,
		200
	};
	// Инициализация размеров контролов для страницы редактирования

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
		gui.rectPage.right / 2 + gui.sizeIndentation.cx,
		gui.rectPage.top + 10 * gui.sizeIndentation.cy,
		200,
		300
	};

	return;
}

// =====================================
// Функция DrawButton
// Перерисовывает кнопку
// =====================================
void DrawButton(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo, HBRUSH hBrush)
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

void SelectPage(HWND hWnd, int ID)
{
	for (size_t i = 0; i < IDPanes.size() - 1; i++)
	{
		if (IDPanes[i] == ID) {
			ShowWindow(GetDlgItem(hWnd, IDPanes[i]), SW_SHOW);
		}
		else {
			ShowWindow(GetDlgItem(hWnd, IDPanes[i]), SW_HIDE);
		}		
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
