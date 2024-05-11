#pragma once

#include <Windows.h>
#include <string>
#include <winnt.h>
#include <codecvt>

struct Settings
{
	LPCWSTR lpFileName = L"./Settings.ini";

	RECT windowRect;

	COLORREF windowColorBckgd;
	COLORREF windowColorBckgdMenu;
	COLORREF windowColorBckgdMenuButt;
	COLORREF windowColorBckgdMenuButtPressed;
	COLORREF windowColorBckgdRecButton;
	COLORREF windowColorBckgdRecButton2;
	COLORREF windowColorText;

	int FontSize = 12;
	std::wstring FontName;

	// Загрузка настроек из INI-файла
	// Обновление значений полей структуры на загруженные настройки
	void LoadSettings()
	{
		TCHAR buffer[1024];
		std::wstring value;
		std::size_t pos;

		// Инициализируем размер окна
		GetPrivateProfileStringW(L"General", L"WindowSize", L"", buffer, 1024, lpFileName);
		readWStringFromBuffer(buffer, value, 1024);

		// Разделяем значение на ширину и высоту окна       
		pos = value.find(L'x');
		if (pos != std::wstring::npos)
		{
			std::wstring widthStr = value.substr(0, pos);
			std::wstring heightStr = value.substr(pos + 1, value.length() - 1);

			if (!widthStr.empty() && !heightStr.empty()) {
				windowRect.right = std::stol(widthStr);
				windowRect.bottom = std::stol(heightStr);
			}
		}

		// Инициализируем позицию окна
		GetPrivateProfileStringW(L"General", L"WindowPosition", L"", buffer, 1024, lpFileName);
		readWStringFromBuffer(buffer, value, 1024);

		// Разделяем значение на ширину и высоту окна       
		pos = value.find(L',');
		if (pos != std::wstring::npos)
		{
			std::wstring positioinXStr = value.substr(0, pos);
			std::wstring positioinYStr = value.substr(pos + 1, value.length() - 1);

			if (!positioinXStr.empty() && !positioinYStr.empty()) {
				windowRect.left = std::stol(positioinXStr);
				windowRect.top = std::stol(positioinYStr);
			}
		}

		// Инициализируем цвета окна
		GetPrivateProfileStringW(L"Colors", L"Background", L"", buffer, 1024, lpFileName);
		readWStringFromBuffer(buffer, value, 1024);

		pos = value.find(L'#');
		if (pos != std::wstring::npos)
		{
			std::wstring colorStr = value.substr(pos + 1, value.length());
			if (!colorStr.empty()) {
				windowColorBckgd = convertWstringToColorRef(colorStr);
			}
		}

		GetPrivateProfileStringW(L"Colors", L"BackgroundMenu", L"", buffer, 1024, lpFileName);
		readWStringFromBuffer(buffer, value, 1024);

		pos = value.find(L'#');
		if (pos != std::wstring::npos)
		{
			std::wstring colorStr = value.substr(pos + 1, value.length());
			if (!colorStr.empty()) {
				windowColorBckgdMenu = convertWstringToColorRef(colorStr);
			}
		}

		GetPrivateProfileStringW(L"Colors", L"BackgroundMenuButton", L"", buffer, 1024, lpFileName);
		readWStringFromBuffer(buffer, value, 1024);

		pos = value.find(L'#');
		if (pos != std::wstring::npos)
		{
			std::wstring colorStr = value.substr(pos + 1, value.length());
			if (!colorStr.empty()) {
				windowColorBckgdMenuButt = convertWstringToColorRef(colorStr);
			}
		}

		GetPrivateProfileStringW(L"Colors", L"BackgroundMenuButtonPressed", L"", buffer, 1024, lpFileName);
		readWStringFromBuffer(buffer, value, 1024);

		pos = value.find(L'#');
		if (pos != std::wstring::npos)
		{
			std::wstring colorStr = value.substr(pos + 1, value.length());
			if (!colorStr.empty()) {
				windowColorBckgdMenuButtPressed = convertWstringToColorRef(colorStr);
			}
		}
		
		GetPrivateProfileStringW(L"Colors", L"BackgroundRecButton", L"", buffer, 1024, lpFileName);
		readWStringFromBuffer(buffer, value, 1024);

		pos = value.find(L'#');
		if (pos != std::wstring::npos)
		{
			std::wstring colorStr = value.substr(pos + 1, value.length());
			if (!colorStr.empty()) {
				windowColorBckgdRecButton = convertWstringToColorRef(colorStr);
			}
		}

		GetPrivateProfileStringW(L"Colors", L"BackgroundRecButton2", L"", buffer, 1024, lpFileName);
		readWStringFromBuffer(buffer, value, 1024);

		pos = value.find(L'#');
		if (pos != std::wstring::npos)
		{
			std::wstring colorStr = value.substr(pos + 1, value.length());
			if (!colorStr.empty()) {
				windowColorBckgdRecButton2 = convertWstringToColorRef(colorStr);
			}
		}

		GetPrivateProfileStringW(L"Colors", L"Text", L"", buffer, 1024, lpFileName);
		readWStringFromBuffer(buffer, value, 1024);

		pos = value.find(L'#');
		if (pos != std::wstring::npos)
		{
			std::wstring colorStr = value.substr(pos + 1, value.length());
			if (!colorStr.empty()) {
				windowColorText = convertWstringToColorRef(colorStr);
			}
		}

		GetPrivateProfileStringW(L"Font", L"FontSize", L"", buffer, 1024, lpFileName);
		readWStringFromBuffer(buffer, value, 1024);

		if (pos != std::wstring::npos)
		{
			std::wstring size = value.substr(0, value.length());
			if (!size.empty()) {
				FontSize = std::stoi(size);
			}
		}

		GetPrivateProfileStringW(L"Font", L"FontName", L"", buffer, 1024, lpFileName);
		readWStringFromBuffer(buffer, value, 1024);

		if (pos != std::wstring::npos)
		{
			std::wstring nameStr = value.substr(0, value.length());
			if (!nameStr.empty()) {
				FontName = nameStr;
			}
		}
	}

	void readWStringFromBuffer(TCHAR* buffer, std::wstring& value, int bufferSize)
	{
		// Находим позицию символа конца строки
		int endPos = 0;
		while (buffer[endPos] != '\0' && endPos < bufferSize)
		{
			endPos++;
		}

		// Преобразуем символы в std::wstring
		value = std::wstring(buffer, endPos);
	}

	COLORREF convertWstringToColorRef(const std::wstring& wstr)
	{
		if (wstr.length() != 6) {
			// Некорректное значение, возвращаем белый цвет
			return RGB(255, 255, 255);
		}

		unsigned long decimalValue = std::stoul(wstr, nullptr, 16);

		int red = 0, green = 0, blue = 0;

		red =  ((decimalValue >> 16) & 0xFF);	// Extract the RR byte
		green = ((decimalValue >> 8) & 0xFF);	// Extract the GG byte
		blue = ((decimalValue) & 0xFF);			// Extract the BB byte

		return RGB(red, green, blue);
	}

	// Сохранение настроек в файл или другое хранилище
	void SaveSettings()
	{

	}

	void setFileSettings(LPCWSTR lpFileName)
	{
		lpFileName = lpFileName;
	}

};