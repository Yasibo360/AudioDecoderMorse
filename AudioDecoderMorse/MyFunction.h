#pragma once

#include "GlobalVar.h"

#include <vector>
#include <string>
#include <array>
#include <locale>

// Функции по созданию класса
ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC WndProc, HBRUSH hbrBackground, LPCUWSTR szWindowClass);

BOOL InitInstance(HINSTANCE, int);

// Функции обработки сообщений
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Функции для создания GUI
void OnCreate(HWND hWnd);

void InitializeUI(HWND hWnd);

void InitControlsRect(RECT rc);

// Функции для управления GUI
void SetEllipticRegion(HWND hWnd);

void DrawButton(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo);

void DrawStatic(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo);

void SelectPane(HWND& hWnd, int ID);

void DrawPane1(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

void DrawPane2(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

void DrawPane3(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

void DrawPane4(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

void createPopupMenuPlot(HWND hWnd, POINT pt);

// Фукнции для работы с файлом
sf_count_t readAudioData(SndfileHandle file, std::vector<std::vector<float>>& samplesByChannel);

sf_count_t readAudioFile(SndfileHandle file, std::vector<std::vector<float>>& samplesByChannel);

void addDictionary(const HWND hWnd, const int IDControl);

std::string openFileDialog();

// Фукнции для работы со звуком
void RecordWithDecode(const HWND hWnd, const int IDControl);

// Фукнции для работы с изображениями
void DrawImage(HWND& hWnd, HINSTANCE& hInst, HDC& hdc, int IDB_BITMAP);

// Фукнции для работы с текстом
void PlayCodeMorse(HWND& hWnd);

void ChangeEditText(HWND& hWnd);

std::vector<std::wstring> Split(const std::wstring& src, const std::wstring& delim);

void CopyIntoBuffer(const HWND hWnd, const int IDControl);

int CountCharactersToEnd(TCHAR* szBuffer, int maxLength);