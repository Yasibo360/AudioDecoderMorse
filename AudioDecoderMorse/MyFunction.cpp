#include "MyFunction.h"

//
//  �������: MyRegisterClass()
//
//  ����: ������������ ����� ����.
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
   �������: InitInstance(HINSTANCE, int)

   ����: ��������� ������ ���������� � ������� ������� ����

   �����������:

		� ���� ������� ������ ���������� ����������� � ���������� ����������, � �����
		��������� � ��������� ������� ���� ���������.
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��������� ������ ���������� � ���������� ����������

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

void OnCreate(HWND hWnd)
{
	// ����� ���� �� �������� ������ � ��������� �������
	MoveWindow(
		hWnd,
		GetSystemMetrics(SM_CXSCREEN) / 2 - settings.windowRect.right / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - settings.windowRect.bottom / 2,
		settings.windowRect.right,
		settings.windowRect.bottom,
		true
	);

	// �������� GUI ����������
	InitializeUI(hWnd);

	// ��������� ������ (��� �������)
	std::setlocale(LC_ALL, "");
}

// ������������� ��������� ���������� ����, �������� ������, ��������� �����, � �.�.
void InitializeUI(HWND hWnd)
{
	RECT rc;

	TCHAR szBuffer[MAX_LOADSTRING]; // ����� ��� �������� ����������� ������
	int countChar = 0;

	GetClientRect(hWnd, &rc);

	InitControlsRect(rc);

	/*
	  �������� ���� ���������
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
		L"�������",
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
		L"����������� �����",
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
		L"����������",
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
		L"� ���������",
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
	  �������� �������
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
	  �������� ��������� ��� 1-�� ������
	*/

	// �������� ������ �������������
	CreateWindowW(
		szRecButtClass,											// Predefined class; Unicode assumed 
		L"����������",											// Button text 
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

	// �������� ���� ��� ������ ���������� �����������
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

	// �������� ��������� ��� 2-�� ������



	// �������� ��������� ��� 3-�� ������
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
		L"���������� ������",
		WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT,
		gui.pane3.rectPane3ListDict.left,
		gui.pane3.rectPane3ListDict.top,
		gui.pane3.rectPane3ListDict.right,
		gui.pane3.rectPane3ListDict.bottom,
		GetDlgItem(hWnd, IDPane3),
		(HMENU)IDPane3ListDict,
		hInst,
		nullptr);

	// ���������� ��������� � ���������� ������
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

	// �������� � ��������� ������
	HICON hIcon = LoadIconW(hInst, MAKEINTRESOURCE(IDI_ICON1));
	SendMessage(GetDlgItem(GetDlgItem(hWnd, IDPane3), IDPane3ButtAddDict), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

	// �������� ��������� ��� 4-�� ������
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

// ������������� ���������� ����������
void InitControlsRect(RECT rc)
{
	// ������������� �������� ��������� ����
	gui.menu.rectMenu = { rc.left, rc.top, rc.right / gui.menu.menuRatio, rc.bottom };
	gui.menu.sizeMenuButton = { gui.menu.rectMenu.right, 50 };

	// ������������� ������� �������
	gui.rectPage = { gui.menu.rectMenu.right, rc.top, rc.right - gui.menu.rectMenu.right, rc.bottom };

	// ������������� �������� ��������� ��� ������� ��������
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
	// ������������� �������� ��������� ��� �������� ��������������

	// ������������� �������� ��������� ��� �������� �����������
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

	// ������������� �������� ��������� ��� �������� ��������
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

	// ������� �������������, ����� ������� ����� ���� ��� (0, 0)
	OffsetRect(&rc, -rc.left, -rc.top);

	// ������� ������ � ���� �������, ������������� ��������������� rc
	hRgn = CreateEllipticRgnIndirect(&rc);

	// ������������� ��������� ������ ��� ������ � ���������� ����������� �����������
	SetWindowRgn(hWnd, hRgn, TRUE);
}

// =====================================
// ������� DrawButton
// �������������� ������
// =====================================
void DrawButton(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo)
{
	TCHAR szBuffer[MAX_LOADSTRING]; // ����� ��� �������� ����������� ������
	int countChar = 0;

	// ������������ ��������� WM_DRAWITEM
	// ������ ���� ��� ��������� �� ������ 
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

	// ���� ������ �������, ������ �� � �������
	// ���������
	if (lpInfo->itemState & ODS_SELECTED)
	{
		HBRUSH hBrushPressed = CreateSolidBrush(settings.windowColorBckgdMenuButtPressed);
		InflateRect(&lpInfo->rcItem, -4, -4);
		FillRect(lpInfo->hDC, &lpInfo->rcItem, hBrushPressed);
		DeleteObject(hBrushPressed);
	}

	// ���� ������ ������� � �� ���� �������
	// ������������, �������� ������� DrawBitmap
	if ((lpInfo->itemAction & ODA_DRAWENTIRE) ||
		(lpInfo->itemAction & ODA_SELECT))
	{
		// ��������� ������ ������
		SetBkMode(lpInfo->hDC, TRANSPARENT);
		SetTextColor(lpInfo->hDC, RGB(255, 255, 255));
		DrawTextW(lpInfo->hDC, szBuffer, countChar, &lpInfo->rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	DeleteObject(hBrush); // �������� �����
}

void DrawStatic(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo)
{
	HDC hdc = lpInfo->hDC;
	RECT rect = lpInfo->rcItem;

	// ���������� ���� ����
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
	TCHAR szBuffer[MAX_LOADSTRING]; // ����� ��� �������� ����������� ������

	// ��������� ������ � ��������������� 104 �� ������� ����� ��������			
	LoadStringW(hInst, 104, szBuffer, MAX_LOADSTRING);
	countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);
	TextOutW(hdc, gui.pane1.rectPane1EditRes.left, gui.pane1.rectPane1EditRes.top - 2 * gui.sizeIndentation.cy, szBuffer, countChar);
}

void DrawPane2(HWND& hWnd, HINSTANCE& hInst, HDC& hdc)
{
	int countChar = 0;
	TCHAR szBuffer[MAX_LOADSTRING]; // ����� ��� �������� ����������� ������


}

void DrawPane3(HWND& hWnd, HINSTANCE& hInst, HDC& hdc)
{
	int countChar = 0;
	TCHAR szBuffer[MAX_LOADSTRING]; // ����� ��� �������� ����������� ������

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

void DrawPane4(HWND& hWnd, HINSTANCE& hInst, HDC& hdc)
{
	int countChar = 0;
	TCHAR szBuffer[MAX_LOADSTRING]; // ����� ��� �������� ����������� ������

	DrawImage(hWnd, hInst, hdc, IDB_BITMAP1);

	LoadStringW(hInst, 108, szBuffer, MAX_LOADSTRING);
	countChar = CountCharactersToEnd(szBuffer, MAX_LOADSTRING);

	// �������� ������ ������
	HFONT hFont = CreateFont(
		40, // ������ ������
		0, // ������ ������
		0, // ���� �������� ������
		0, // ���� ������� ������
		FW_BOLD, // ������� ������
		FALSE, // ������
		FALSE, // �������������
		FALSE, // ������������
		DEFAULT_CHARSET, // ����� ��������
		OUT_DEFAULT_PRECIS, // �������� ������
		CLIP_DEFAULT_PRECIS, // �������� ���������
		DEFAULT_QUALITY, // �������� ������
		DEFAULT_PITCH | FF_SWISS, // ���������� ������
		L"Arial" // ��� ������
	);

	// ����� ������ ������ ��� ��������� ����������
	HGDIOBJ oldFont = SelectObject(hdc, hFont);

	TextOutW(
		hdc,
		gui.rectPage.right / 2 - 12 * gui.sizeIndentation.cx,
		gui.rectPage.top + 6 * gui.sizeIndentation.cy,
		szBuffer,
		countChar
	);

	// �������� �������� ����� � ����� ������
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));

	// ���������� �������������� ������
	RECT rectText = {
		gui.rectPage.right - 450 - 4 * gui.sizeIndentation.cy,
		gui.rectPage.top + 14 * gui.sizeIndentation.cy,
		gui.rectPage.right - 4 * gui.sizeIndentation.cy,
		gui.rectPage.top + 300 + 14 * gui.sizeIndentation.cy
	};
	FillRect(hdc, &rectText, brush);

	// ����� ������ ������ ��� ��������� ����������
	SelectObject(hdc, oldFont);

	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT); // ��������� ������ ����������� ����	
	WCHAR text[] = L"������ ���������� ��������������� � ������ ������� ��� �� ����������� 09.03.01\
				\n\n������ ���������� ������������ �� ���� ����������� �����.\
				\n��� �������� �������������� ����������� � ������������� ���������� �� ������ �����.";
	rectText = {
		gui.rectPage.right - 450 - 3 * gui.sizeIndentation.cy,
		gui.rectPage.top + 15 * gui.sizeIndentation.cy,
		gui.rectPage.right - 5 * gui.sizeIndentation.cy,
		gui.rectPage.top + 300 + 13 * gui.sizeIndentation.cy
	};
	DrawTextW(hdc, text, -1, &rectText, DT_LEFT | DT_TOP | DT_WORDBREAK);

	// �������� �����
	DeleteObject(brush);

	// ������������ ������ ����� �������������
	DeleteObject(hFont);
}

void RecordWithDecode()
{
	if (!recorder.IsRecording()) {
		recorder.StartRecording();
	}
	else {
		recorder.StopRecording();
		// �������� ����������� �������������� �� ����������� �����

	}
}

void DrawImage(HWND& hWnd, HINSTANCE& hInst, HDC& hdc, int IDB_BITMAP)
{
	HBITMAP hBitmap = (HBITMAP)LoadImage(
		hInst, // ���������� ���������� ����������
		MAKEINTRESOURCE(IDB_BITMAP1), // ������������� ������� �����������
		IMAGE_BITMAP, // ��� �����������
		0, 0, // ������ � ������ ������������ ����������� (0 ��� ���������������)
		LR_DEFAULTCOLOR // ����� ��������
	);

	// �������� COMPATIBLE DC ��� ���������
	HDC hdcMem = CreateCompatibleDC(hdc);
	if (hdcMem == NULL) {
		ReleaseDC(hWnd, hdc);
		return;
	}

	// ����� ����������� � COMPATIBLE DC
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

	BITMAP bm;
	GetObject(hBitmap, sizeof(bm), (LPSTR)&bm);

	// ��������� ����������� � ����
	BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	// ������� ��������
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
	morse.PlayMorseCode(morseCode);
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

		// ����������� ������ � ����� ������
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
