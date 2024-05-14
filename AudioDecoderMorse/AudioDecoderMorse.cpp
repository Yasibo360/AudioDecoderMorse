// Decoder Morze.cpp : Определяет точку входа для приложения.
//

#include "GlobalVar.h"
#include "MyFunction.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_AUDIODECODERMORSE, szWindowClass, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_AUDIORECODER, szRecButtClass, MAX_LOADSTRING);

	// Считываем настройки программы из файла настроек
	settings.LoadSettings();

	// Регистрация классов
	MyRegisterClass(hInstance, WndProc, CreateSolidBrush(settings.windowColorBckgd), szWindowClass);
	MyRegisterClass(hInstance, ButtonProc, CreateSolidBrush(settings.windowColorBckgd), szRecButtClass);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUDIODECODERMORSE));
	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

/*
  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)

  ЦЕЛЬ: Обрабатывает сообщения в главном окне.

  WM_COMMAND  - обработать меню приложения
  WM_PAINT    - Отрисовка главного окна
  WM_DESTROY  - отправить сообщение о выходе и вернуться
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rc;
	static sf::Texture texture;
	static std::string fileName;

	switch (message)
	{
	case WM_CREATE:
	{
		OnCreate(hWnd);
	}
	break;
	case WM_COMMAND:
	{
		switch HIWORD(wParam)
		{
		case BN_CLICKED:
		{
			switch (LOWORD(wParam))
			{
			case IDButtPane1:
			{
				SelectPane(hWnd, IDPane1);
			}
			break;
			case IDButtPane2:
			{
				SelectPane(hWnd, IDPane2);
			}
			break;
			case IDButtPane3:
			{
				SelectPane(hWnd, IDPane3);
			}
			break;
			case IDButtPane4:
			{
				SelectPane(hWnd, IDPane4);
			}
			break;
			case IDPane3ButtCopyText:
			{
				CopyIntoBuffer(hWnd, IDPane3EditText);
			}
			break;
			case IDPane3ButtPlayCode:
			{
				PlayCodeMorse(hWnd);
			}
			break;
			case IDPane3ButtCopyCode:
			{
				CopyIntoBuffer(hWnd, IDPane3EditCode);
			}
			break;
			case IDPane3ButtAddDict:
			{
				addDictionary(hWnd, IDPane3ListDict);
			}
			break;
			case IDMenuItem1:
			{
				fileName = openFileDialog();

				SndfileHandle file(fileName);

				if (file.error() != 0) {
					OutputDebugString(L"Ошибка при открытии файла: ");
					OutputDebugString(std::to_wstring(sf_error(file.rawHandle())).c_str());
				}

				std::vector<std::vector<float>> samplesByChannel;
				sf_count_t frames_t;
				frames_t = readAudioData(file, samplesByChannel);

				if (frames_t != 0) {
					std::size_t found = fileName.find_last_of("/\\"); // Находим последний символ '/' или '\'
					std::string just_fileName = fileName.substr(found + 1); // Обрезаем путь до последнего символа '/' или '\'

					GetClientRect(GetDlgItem(hWnd, IDPane2Plot), &rc);

					plot = Plot_AmpTime(sf::Vector2f(rc.right, rc.bottom), sf::Vector2f(0, 0), just_fileName, "arial.ttf");

					// Вычисляем время для каждого отсчета
					float samplerate = file.samplerate();
					float frames = frames_t / file.channels();
					std::vector<float> time(frames);

					for (int i = 0; i < frames; i++) {
						time[i] = i / samplerate;
					}

					// Вычисляем период сигнала
					float period = 1.0 / samplerate;

					// Получаем начальное и конечное время
					float timeStart = time.front();
					float timeEnd = time.back();

					std::vector<std::pair<float, float>> widePeaks = morse.findWidePeaksInAudioFile(fileName);

					auto boundFunction = std::bind(&Plot_AmpTime::graphAmplitudeFromTime, &plot, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
					plot.addCurve("Right channel", boundFunction, time, samplesByChannel[0]);
					plot.addWidePeaks(widePeaks, timeStart, timeEnd);

					std::wstring morseCode;
					std::wstring morseChar;
					std::vector<std::pair<char, float>> peakDurations;

					peakDurations = morse.findPeakDurations(widePeaks, 2);
					morseCode = morse.peakDurationsToMorse(peakDurations);
					morseChar = morse.morseToChar(morseCode);

					SetWindowTextW(GetDlgItem(hWnd, IDPane2EditCode), L"");
					SetWindowTextW(GetDlgItem(hWnd, IDPane2EditText), L"");

					WCHAR buffer[MAX_EDITSTRING];
					if (morseCode.length() != 0)
					{
						for (size_t i = 0; i < morseCode.length(); i++)
						{
							buffer[i] = morseCode[i];
						}
						buffer[morseCode.length()] = '\0';
						SetWindowTextW(GetDlgItem(hWnd, IDPane2EditCode), buffer);
					}
					if (morseChar.length() != 0)
					{
						for (size_t i = 0; i < morseChar.length(); i++)
						{
							buffer[i] = morseChar[i];
						}
						buffer[morseChar.length()] = '\0';
						SetWindowTextW(GetDlgItem(hWnd, IDPane2EditText), buffer);
					}

					// Создаем экземпляр sf::RenderWindow с контекстом устройства окна
					sf::RenderWindow window(GetDlgItem(hWnd, IDPane2Plot));

					// Рисуем график
					window.draw(plot);

					// Отображаем изменения
					window.display();

					// Сохраняем изображение графика в файл
					sf::Vector2u winSize = window.getSize();
					texture.create(winSize.x, winSize.y);

					// Сохраняем содержимое окна в текстуру
					texture.update(window);
				}
			}
			break;
			case IDMenuItem2:
			{
				processPlotData(fileName, "Threshold value", 0, sf::Color::Red, hWnd, IDPane2Plot, plot);
			}
			break;
			case IDMenuItem3:
			{
				processPlotData(fileName, "Mean value", 1, sf::Color(255, 140, 0, 255), hWnd, IDPane2Plot, plot);
			}
			break;			
			case IDMenuItem4:
			{
				processPlotData(fileName, "Mean positive value", 2, sf::Color(255, 215, 0, 255), hWnd, IDPane2Plot, plot);
			}
			break;
			case IDMenuItem5:
			{
				processPlotData(fileName, "Mean negative value", 3, sf::Color(255, 215, 0, 255), hWnd, IDPane2Plot, plot);
			}
			break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case EN_CHANGE:
		{
			switch LOWORD(wParam)
			{
			case IDPane3EditText:
			{
				if (GetWindowTextLength(GetDlgItem(hWnd, IDPane3EditText)) < SendMessageW(GetDlgItem(hWnd, IDPane3EditText), EM_GETLIMITTEXT, 0, 0) - 10)
				{
					ChangeEditText(hWnd);
				}
			}
			break;
			case IDPane3EditCode:
			{
				if (SendMessageW(GetDlgItem(hWnd, IDPane3EditCode), EM_LINELENGTH, 0, 0) < SendMessageW(GetDlgItem(hWnd, IDPane3EditCode), EM_GETLIMITTEXT, 0, 0))
				{

				}
			}
			break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case CBN_DROPDOWN:
	{
		if (LOWORD(wParam) == IDPane3ListDict)
		{
			// Автоматическое раскрытие выпадающего списка		
			SendMessage(GetDlgItem(GetDlgItem(hWnd, IDPane3), IDPane3ListDict), CB_SHOWDROPDOWN, TRUE, 0);
		}
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rc);

		// Установка прозрачного фона для текста
		SetBkMode(hdc, TRANSPARENT);

		// Установка цвета текста
		SetTextColor(hdc, RGB(255, 255, 255)); // Установка цвета текста

		if (hWnd == GetParent(GetDlgItem(hWnd, IDPane1ButtonRec))) {
			DrawPane1(hWnd, hInst, hdc);
		}

		if (hWnd == GetParent(GetDlgItem(hWnd, IDPane2Plot))) {
			DrawPane2(hWnd, hInst, hdc);
			DrawPlot(hWnd, texture);
		}

		if (hWnd == GetParent(GetDlgItem(hWnd, IDPane3EditText))) {
			DrawPane3(hWnd, hInst, hdc);
		}

		if (hWnd == GetParent(GetDlgItem(hWnd, IDPane4Picture))) {
			DrawPane4(hWnd, hInst, hdc);
		}

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CONTEXTMENU:
	{
		if (GetDlgItem(hWnd, IDPane2Plot) != NULL) {
			// Отображение контекстного меню в указанной позиции
			POINT pt;
			POINT ptPlot;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);

			ptPlot = pt;

			// Преобразование координат экрана в координаты клиентской области
			ScreenToClient(GetDlgItem(hWnd, IDPane2Plot), &ptPlot);

			RECT rectPlot = gui.pane2.rectPane2Plot;
			rectPlot.left = 0;
			rectPlot.top = 0;

			// Проверка, находится ли щелчок внутри области графика
			if (PtInRect(&rectPlot, ptPlot)) {
				// Отображение контекстного меню в указанной позиции
				createPopupMenuPlot(hWnd, pt);
			}
		}
	}
	break;
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;

		switch (lpDrawItem->CtlID)
		{
		case IDBgrMenu:
		{
			DrawStatic(hInst, lpDrawItem);
		}
		break;
		default:
			DrawButton(hInst, lpDrawItem);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик рисования кнопки
LRESULT CALLBACK ButtonProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rc;

	switch (message)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rc);

		// Создание нового шрифта
		HFONT hFont = CreateFont(
			20,			// Высота шрифта
			0,			// Ширина шрифта
			0,			// Угол поворота шрифта
			0,			// Угол наклона шрифта
			FW_DEMIBOLD,	// Толщина шрифта
			FALSE,		// Курсив
			FALSE,		// Подчеркивание
			FALSE,		// Зачеркивание
			DEFAULT_CHARSET,			// Набор символов
			OUT_DEFAULT_PRECIS,			// Точность вывода
			CLIP_DEFAULT_PRECIS,		// Точность отсечения
			DEFAULT_QUALITY,			// Качество шрифта
			DEFAULT_PITCH | FF_SWISS,	// Начертание шрифта
			L"Arial"	// Имя шрифта
		);

		// Выбор нового шрифта для контекста устройства
		SelectObject(hdc, hFont);

		// Рисуем фон кнопки
		HBRUSH hBrush = CreateSolidBrush(settings.windowColorBckgdRecButton);
		HRGN hRegion = CreateEllipticRgn(0, 0, 0, 0);

		GetWindowRgn(hWnd, hRegion);
		FillRgn(hdc, hRegion, hBrush);

		// Рисуем рамку эллипса другим цветом
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Создаем красную пенсиль
		hBrush = CreateSolidBrush(settings.windowColorBckgdRecButton2);

		SelectObject(hdc, hPen); // Выбираем пенсиль для контура
		FrameRgn(hdc, hRegion, hBrush, 5, 5); // Рисуем рамку эллипса

		// Рисуем текст кнопки
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));

		WCHAR nameButton[MAX_LOADSTRING];
		LoadStringW(hInst, IDS_STRING115, nameButton, MAX_LOADSTRING);

		DrawTextW(hdc, nameButton, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// Освобождаем ресурсы
		DeleteObject(hPen);
		DeleteObject(hRegion);
		DeleteObject(hBrush);
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		// Получаем контекст устройства кнопки
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &rc);

		// Создание нового шрифта
		HFONT hFont = CreateFont(
			20,			// Высота шрифта
			0,			// Ширина шрифта
			0,			// Угол поворота шрифта
			0,			// Угол наклона шрифта
			FW_BOLD,	// Толщина шрифта
			FALSE,		// Курсив
			TRUE,		// Подчеркивание
			FALSE,		// Зачеркивание
			DEFAULT_CHARSET,			// Набор символов
			OUT_DEFAULT_PRECIS,			// Точность вывода
			CLIP_DEFAULT_PRECIS,		// Точность отсечения
			DEFAULT_QUALITY,			// Качество шрифта
			DEFAULT_PITCH | FF_SWISS,	// Начертание шрифта
			L"Arial"	// Имя шрифта
		);

		// Выбор нового шрифта для контекста устройства
		SelectObject(hdc, hFont);

		HBRUSH hBrush = CreateSolidBrush(settings.windowColorBckgdRecButton2);
		HRGN hRegion = CreateEllipticRgn(0, 0, 0, 0);

		GetWindowRgn(hWnd, hRegion);
		FillRgn(hdc, hRegion, hBrush);

		// Рисуем текст кнопки
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));

		WCHAR nameButton[MAX_LOADSTRING];
		LoadStringW(hInst, IDS_STRING115, nameButton, MAX_LOADSTRING);

		DrawText(hdc, nameButton, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// Освобождаем контекст устройства кнопки
		ReleaseDC(hWnd, hdc);
		DeleteObject(hRegion);
		DeleteObject(hBrush);
		DeleteObject(hFont);
	}
	break;
	case WM_LBUTTONUP:
	{
		// Перерисовываем кнопку
		InvalidateRect(hWnd, nullptr, FALSE);

		RecordWithDecode(hWnd, IDPane1EditRes);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}