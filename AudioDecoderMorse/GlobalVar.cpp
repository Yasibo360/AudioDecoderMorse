#include "GlobalVar.h"

// ���������� ���������� ����:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����
LPCWSTR szWndClassPane;

// ���������� ��������� ��������
Settings settings;

// ���������� ��������� GUI
My_GUI gui;

// ��� ���������� ����������
Morse�ode morse;
AudioRecorder recorder("recorded.wav");