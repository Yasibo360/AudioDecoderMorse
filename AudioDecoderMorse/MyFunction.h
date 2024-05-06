#pragma once
#include <vector>
#include <string>
#include <array>
#include <Windows.h>

std::vector<std::wstring> Split(const std::wstring& src, const std::wstring& delim);
void CopyIntoBuffer(const HWND hWnd, const int IDControl);

std::wstring OpenFileDialog();