#pragma once

#include <vector>
#include <string>
#include <array>
#include <locale>

#include "GlobalVar.h"

// Функции для создания GUI
void OnCreate(HWND hWnd, HBRUSH hBrush);

void InitializeUI(HWND hWnd, HBRUSH hBrush);

void InitControlsRect(RECT rc);

// Функции для управления GUI
void DrawButton(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo, HBRUSH hBrush);

void SelectPage(HWND hWnd, int ID);

// Фукнции для работы с текстом
std::vector<std::wstring> Split(const std::wstring& src, const std::wstring& delim);

void CopyIntoBuffer(const HWND hWnd, const int IDControl);

int CountCharactersToEnd(TCHAR* szBuffer, int maxLength);