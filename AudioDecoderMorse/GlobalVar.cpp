#include "GlobalVar.h"

// Глобальные переменные окна:
HINSTANCE hInst;                                // Текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // Имя класса главного окна
LPCWSTR szWndClassPane;

// Глобальная структура настроек
Settings settings;

// Глобальная структура GUI
My_GUI gui;

// Мои глобальные переменные
MorseСode morse;
AudioRecorder recorder("recorded.wav");