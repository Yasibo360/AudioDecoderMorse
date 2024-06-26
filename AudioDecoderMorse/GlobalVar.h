#pragma once

#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>

#include <thread>
#include <future>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp >
#include "Resource.h"
#include "framework.h"

#include "Settings.h"
#include "ControlsID.h"
#include "My_GUI.h"

#include "Plot/Plot_AmpTime.h"
#include "Morse�ode.h"
#include "AudioRecorder.h"
#include "AudioDecoderMorse.h"
#include "MyFunction.h"

#define MAX_LOADSTRING 256
#define MAX_EDITSTRING 1000

// ���������� ���������� ����:
extern HINSTANCE hInst;                                // ������� ���������
extern WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
extern WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����
extern WCHAR szRecButtClass[MAX_LOADSTRING];           // ��� ������ ������ ���������

extern HWND hWndPane1;
extern HWND hWndPane2;
extern HWND hWndPane3;
extern HWND hWndPane4;

// ���������� ��������� ��������
extern Settings settings;

// ���������� ��������� GUI
extern My_GUI gui;

// ��� ���������� ����������
extern Morse�ode morse;
extern AudioRecorder recorder;
extern std::thread threadPlaingMorse;
extern Plot_AmpTime plot;