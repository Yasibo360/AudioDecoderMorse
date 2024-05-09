#pragma once

#define NOMINMAX

#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Resource.h"
#include "framework.h"

#include "Settings.h"
#include "ControlsID.h"
#include "My_GUI.h"

#include "MorseСode.h"
#include "AudioRecorder.h"
#include "AudioDecoderMorse.h"
#include "MyFunction.h"

#define MAX_LOADSTRING 256
#define MAX_EDITSTRING 1000

// Глобальные переменные окна:
extern HINSTANCE hInst;                                // Текущий экземпляр
extern WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
extern WCHAR szWindowClass[MAX_LOADSTRING];            // Имя класса главного окна
extern WCHAR szRecButtClass[MAX_LOADSTRING];           // Имя класса кнопки прослушки

extern HWND hWndPane1;
extern HWND hWndPane2;
extern HWND hWndPane3;
extern HWND hWndPane4;

// Глобальная структура настроек
extern Settings settings;

// Глобальная структура GUI
extern My_GUI gui;

// Мои глобальные переменные
extern MorseСode morse;
extern AudioRecorder recorder;