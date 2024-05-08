#pragma once

#include <Windows.h>
#include "Resource.h"
#include "framework.h"

#include "Settings.h"
#include "ControlsID.h"
#include "My_GUI.h"

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

// ���������� ��������� ��������
extern Settings settings;

// ���������� ��������� GUI
extern My_GUI gui;

// ��� ���������� ����������
extern Morse�ode morse;
extern AudioRecorder recorder;