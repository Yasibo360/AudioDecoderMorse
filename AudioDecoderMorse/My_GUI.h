#pragma once

#include <windows.h>

// ������� ��� ���� � ��� ������
struct Menu
{
	const int ID = IDBgrMenu;

	// ��������� ���������� ����������� ���� � ������� �������
	const int menuRatio = 8;

	RECT rectMenu;
	SIZE sizeMenuButton;
};

// ������� ��������� ��� ������� ��������
struct Pane1 
{
	const int ID = IDPane1;

	RECT rectPane1ButtonRec;
	RECT rectPane1EditRes;
};

// ������� ��������� ��� �������� ��������������
struct Pane2
{
	const int ID = IDPane2;

	RECT rectPane2Plot;
	RECT rectPane2ButtPlay;
	RECT rectPane2ButtReset;
	RECT rectPane2EditCode;
	RECT rectPane2EditText;
};

// ������� ��������� ��� �������� �����������
struct Pane3
{
	const int ID = IDPane3;

	const int indOnTopPane3 = 8; // ��������� ��� �������������

	RECT rectPane3EditText;
	RECT rectPane3EditCode;
	RECT rectPane3ButtCopyText;
	RECT rectPane3ButtPlayCode;
	RECT rectPane3ButtCopyCode;
	RECT rectPane3ListDict;
	RECT rectPane3ButtAddDic;
};

// ������� ��������� ��� �������� ��������
struct Pane4
{
	const int ID = IDPane4;

	RECT rectPane4Picture;
};

// ������� ��������� ��� GUI ����������
struct My_GUI
{
	const SIZE sizeButton = { 40, 40 };
	const SIZE sizeIndentation = { 10, 10 };

	// ��������� ����
	Menu menu;

	// ������ ���� ������� ��������
	RECT rectPage;

	// ��������� �������
	Pane1 pane1;
	Pane2 pane2;
	Pane3 pane3;
	Pane4 pane4;
};