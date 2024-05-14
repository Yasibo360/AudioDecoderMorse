#pragma once

#include "GlobalVar.h"

#include <vector>
#include <string>
#include <array>
#include <locale>

// ������� �� �������� ������
ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC WndProc, HBRUSH hbrBackground, LPCUWSTR szWindowClass);

BOOL InitInstance(HINSTANCE, int);

// ������� ��������� ���������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// ������� ��� �������� GUI
void OnCreate(HWND hWnd);

void InitializeUI(HWND hWnd);

void InitControlsRect(RECT rc);

// ������� ��� ���������� GUI
void SetEllipticRegion(HWND hWnd);

void DrawButton(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo);

void DrawStatic(HINSTANCE hInst, LPDRAWITEMSTRUCT lpInfo);

void SelectPane(HWND& hWnd, int ID);

void DrawPane1(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

void DrawPane2(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

void DrawPane3(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

void DrawPane4(HWND& hWnd, HINSTANCE& hInst, HDC& hdc);

void DrawPlot(HWND& hWnd, sf::Texture& texture);

void processPlotData(const std::string& fileName, const std::string& curveName, const int typeLine, const sf::Color& color, HWND hWnd, int controlID, Plot_AmpTime& plot);

void createPopupMenuPlot(HWND hWnd, POINT pt);

// ������� ��� ������ � ������
sf_count_t readAudioData(SndfileHandle file, std::vector<std::vector<float>>& samplesByChannel);

void addDictionary(const HWND hWnd, const int IDControl);

std::string openFileDialog();

// ������� ��� ������ �� ������
void RecordWithDecode(const HWND hWnd, const int IDControl);

// ������� ��� ������ � �������������
void DrawImage(HWND& hWnd, HINSTANCE& hInst, HDC& hdc, int IDB_BITMAP);

// ������� ��� ������ � �������
void PlayCodeMorse(HWND& hWnd);

void ChangeEditText(HWND& hWnd);

std::vector<std::wstring> Split(const std::wstring& src, const std::wstring& delim);

void CopyIntoBuffer(const HWND hWnd, const int IDControl);

int CountCharactersToEnd(TCHAR* szBuffer, int maxLength);