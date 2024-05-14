#pragma once

#include <windows.h>

// Размеры для меню и его кнопок
struct Menu
{
	const int ID = IDBgrMenu;

	// Константа определяет соотношение меню к рабочей области
	const int menuRatio = 8;

	RECT rectMenu;
	SIZE sizeMenuButton;
};

// Размеры контролов для главной страницы
struct Pane1 
{
	const int ID = IDPane1;

	RECT rectPane1ButtonRec;
	RECT rectPane1EditRes;
};

// Размеры контролов для страницы редактирования
struct Pane2
{
	const int ID = IDPane2;

	RECT rectPane2Plot;
	RECT rectPane2ButtPlay;
	RECT rectPane2ButtReset;
	RECT rectPane2EditCode;
	RECT rectPane2EditText;
};

// Размеры контролов для страницы переводчика
struct Pane3
{
	const int ID = IDPane3;

	const int indOnTopPane3 = 8; // константа для центрирования

	RECT rectPane3EditText;
	RECT rectPane3EditCode;
	RECT rectPane3ButtCopyText;
	RECT rectPane3ButtPlayCode;
	RECT rectPane3ButtCopyCode;
	RECT rectPane3ListDict;
	RECT rectPane3ButtAddDic;
};

// Размеры контролов для страницы описания
struct Pane4
{
	const int ID = IDPane4;

	RECT rectPane4Picture;
};

// Главная структура для GUI приложения
struct My_GUI
{
	const SIZE sizeButton = { 40, 40 };
	const SIZE sizeIndentation = { 10, 10 };

	// Структура меню
	Menu menu;

	// Размер всех страниц одинаков
	RECT rectPage;

	// Структуры страниц
	Pane1 pane1;
	Pane2 pane2;
	Pane3 pane3;
	Pane4 pane4;
};