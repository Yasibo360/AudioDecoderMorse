#include "MyFunction.h"

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

std::wstring OpenFileDialog()
{
    OPENFILENAMEW ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAMEW));

    wchar_t fileName[MAX_PATH];
    ZeroMemory(fileName, sizeof(fileName));

    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.hwndOwner = NULL; // Указывается только в случае использования внутри окна
    ofn.lpstrFilter = L"All Files\0*.*\0Wave Files\0*.wav\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	std::wstring filePath;
    if (GetOpenFileNameW(&ofn) == TRUE)
    {
        filePath = ofn.lpstrFile;
    }

    return filePath;
}