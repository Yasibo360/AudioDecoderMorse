#pragma once

#include <vector>
#include <string>
#include <array>
#include <locale>

#include "GlobalVar.h"

// Функции по созданию класса
ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC WndProc, HBRUSH hbrBackground, LPCUWSTR szWindowClass);

BOOL InitInstance(HINSTANCE, int);

// Функция обработки сообщений
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// Функции для создания GUI
void OnCreate(HWND hWnd);

void InitializeUI(HWND hWnd);

void InitControlsRect(RECT rc);

// Функции для управления GUI
void DrawButton(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo);

void DrawStatic(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo);

void SelectPane(HWND& hWnd, int ID);

void DrawPane1(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

void DrawPane2(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

void DrawPane3(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

void DrawPane4(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

// Фукнции для работы со звуком
void RecordWithDecode();

// Фукнции для работы с изображениями
void DrawImage(HWND& hWnd, HINSTANCE& hInst, HDC& hdc, int IDB_BITMAP);

// Фукнции для работы с текстом
void PlayCodeMorse(HWND& hWnd);

void ChangeEditText(HWND& hWnd);

std::vector<std::wstring> Split(const std::wstring& src, const std::wstring& delim);

void CopyIntoBuffer(const HWND hWnd, const int IDControl);

int CountCharactersToEnd(TCHAR* szBuffer, int maxLength);