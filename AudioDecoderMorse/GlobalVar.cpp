#include "GlobalVar.h"

// ���������� ���������� ����:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����
WCHAR szRecButtClass[MAX_LOADSTRING];           // ��� ������ ������ ���������

HWND hWndPane1;
HWND hWndPane2;
HWND hWndPane3;
HWND hWndPane4;

// ���������� ��������� ��������
Settings settings;

// ���������� ��������� GUI
My_GUI gui;

// ��� ���������� ����������
Morse�ode morse;
AudioRecorder recorder("recorded.wav");
std::thread threadPlaingMorse; 