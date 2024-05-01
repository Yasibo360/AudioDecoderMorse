#include "AudioAnalizer.h"
#include "AudioGraph.h"

AudioAnalizer::AudioAnalizer(const std::string& filename) {
	// Чтение файла WAV
	wavReaderWriter.OpenFileForRead(filename);

	int bitsPerSample = sizeof(short) * 8;

	// Инициализация sizeBuffer
	int sizeBuffer = wavReaderWriter.GetSF_Info().samplerate * wavReaderWriter.GetSF_Info().channels * bitsPerSample / 8;

	audioData.resize(sizeBuffer);
	wavReaderWriter.ReadData(audioData.data(), audioData.size());

	wavReaderWriter.CloseFile();
}

AudioAnalizer::~AudioAnalizer() {}

// Реализация построения графика амплитуды по времени
void AudioAnalizer::PlotAmplitudeOverTime(HWND hWnd) {
	int samplerate = wavReaderWriter.GetSF_Info().samplerate;
	int channels = wavReaderWriter.GetSF_Info().channels;

	// Преобразование HWND в sf::RenderWindow*
	sf::WindowHandle handle = reinterpret_cast<sf::WindowHandle>(hWnd);
	sf::RenderWindow window(handle);

	// Установка цвета фона окна
	window.clear(sf::Color(8, 111, 161)); //brushPane

	unsigned int width = window.getSize().x;
	unsigned int height = window.getSize().y;

	// Рассчитываем размеры прямоугольной области с отступами
	float marginX = width * 0.05f; // Отступ слева и справа 5% от ширины
	float marginY = height * 0.2f; // Отступ сверху и снизу, чтобы выровнять

	std::vector<short> left_channel;
	if (audioData.size() % 2 == 0)
	{
		for (size_t i = 0; i < audioData.size(); i += 2)
		{
			left_channel.push_back(audioData[i]);
		}
	}




	std::string audioFile = "recorded.wav";
	AudioGraph audioGraph(audioData, sf::Vector2i(0, 0));


	sf::RenderWindow window1(sf::VideoMode(800, 600), "Audio Graph");
	window1.setFramerateLimit(60);


	while (window1.isOpen())
	{
		// Обработка событий
		sf::Event event;
		while (window1.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window1.close();
		}

		// Отрисовка
		window1.clear();
		window1.draw(audioGraph);
		window1.display();

		// Обновление аудиографика
		audioGraph.update();
	}



	/*

	// Создаем прямоугольник, который определяет область рисования
	sf::FloatRect drawArea(sf::Vector2f(marginX, marginY), sf::Vector2f(width - 2 * marginX, height - 2 * marginY));

	// Создаем тонкую черную рамку вокруг видимой области
	sf::RectangleShape borderRect(sf::Vector2f(drawArea.getSize().x + 1, drawArea.getSize().y + 1));
	borderRect.setPosition(drawArea.getPosition().x - 1, drawArea.getPosition().y - 1);
	borderRect.setFillColor(sf::Color::Transparent);
	borderRect.setFillColor(sf::Color::White);
	borderRect.setOutlineColor(sf::Color::Black);
	borderRect.setOutlineThickness(1.0f);

	// Находим максимальное значение амплитуды
	float maxAmplitude = *std::max_element(audioData.begin(), audioData.end());
	float normalize = (drawArea.getSize().y / 2) / maxAmplitude;

	// Создаем график зависимости амплитуды от времени
	sf::VertexArray graph(sf::LinesStrip);
	for (int i = 0; i < audioData.size(); ++i) {
		float x = i * (drawArea.getSize().x / audioData.size()); // Распределяем точки по оси X
		float y = audioData[i] * normalize; // Нормализуем значения амплитуды
		y = std::min(y, drawArea.getSize().y); // Ограничиваем значение по высоте области рисования
		y += drawArea.getPosition().y + drawArea.getSize().y / 2; // Сдвигаем на позицию области рисования
		graph.append(sf::Vertex(sf::Vector2f(drawArea.getPosition().x + x, y), sf::Color::Red)); // Цвет графика
	}

	// Добавляем название графика
	sf::Text title("График амплитуды по времени", sf::Font(), 20);
	title.setFillColor(sf::Color::Black);
	title.setPosition(drawArea.getPosition().x + drawArea.getSize().x / 2,
		drawArea.getPosition().y - drawArea.getSize().y / 10); // Позиция над рамкой

	// Создаем сетку серого цвета
	sf::VertexArray grid(sf::Lines);
	grid.resize(2 * (drawArea.getSize().x + drawArea.getSize().y)); // Указываем нужное количество вершин
	grid.setPrimitiveType(sf::Lines);
	grid[0].position = sf::Vector2f(drawArea.getPosition().x, drawArea.getPosition().y);
	grid[1].position = sf::Vector2f(drawArea.getPosition().x + drawArea.getSize().x, drawArea.getPosition().y);
	grid[2].position = sf::Vector2f(drawArea.getPosition().x, drawArea.getPosition().y);
	grid[3].position = sf::Vector2f(drawArea.getPosition().x, drawArea.getPosition().y + drawArea.getSize().y);

	// Задаем серый цвет с прозрачностью
	for (int i = 0; i < grid.getVertexCount(); ++i) {
		grid[i].color = sf::Color(200, 200, 200, 150);
	}

	// Добавляем подписи для сетки
	// Для вертикальной сетки слева
	for (int i = 0; i <= 10; ++i) {
		sf::Text label(std::to_string(i), sf::Font(), 12);
		label.setFillColor(sf::Color::Black);
		label.setPosition(drawArea.getPosition().x - 10, drawArea.getPosition().y + i * drawArea.getSize().y / 10);
	}

	// Для горизонтальной сетки снизу
	for (int i = 0; i <= audioData.size(); i += audioData.size() / 10) {
		sf::Text label(std::to_string(i), sf::Font(), 12);
		label.setFillColor(sf::Color::Black);
		label.setPosition(drawArea.getPosition().x + i * drawArea.getSize().x / audioData.size(), drawArea.getPosition().y + drawArea.getSize().y + 10);
	}

	// Добавляем подписи осей
	sf::Text xAxisLabel("Время", sf::Font(), 12);
	xAxisLabel.setFillColor(sf::Color::Black);
	xAxisLabel.setPosition(drawArea.getPosition().x + drawArea.getSize().x - 30, drawArea.getPosition().y + drawArea.getSize().y + 10);

	sf::Text yAxisLabel("Амплитуда", sf::Font(), 12);
	yAxisLabel.setFillColor(sf::Color::Black);
	yAxisLabel.setPosition(drawArea.getPosition().x - 50, drawArea.getPosition().y + drawArea.getSize().y / 2);

	// Отрисовка графика и осей
	window.draw(borderRect); // Добавьте прямоугольник в окно
	window.draw(grid);
	//window.draw(title);
	//window.draw(xAxisLabel);
	//window.draw(yAxisLabel);
	window.draw(graph);
	

	// Отображение нарисованного на экране
	window.display();*/
}

// Реализация построения графика амплитуды по частоте
void AudioAnalizer::PlotAmplitudeOverFrequency() {

}

// Реализация поиска пиковых значений
void AudioAnalizer::FindPeaks() {

}

// Реализация вычисления среднего значения на промежутке сигнала
double AudioAnalizer::CalculateAverage() {
	double average_val = 0;
	for (const auto& data : audioData) {
		average_val += data;
	}
	average_val = average_val / audioData.size();
	return average_val;
}

// Реализация метода для определения широких пиков
std::vector<std::pair<int, int>> AudioAnalizer::FindWidePeaks() {
	std::vector<std::pair<int, int>> widePeaks;
	std::vector<std::pair<double, double>> widePeak;
	// Вычисляем среднее значение для положительной и отрицательной частей амплатуды
	double mean_positive_left = 0;
	double mean_negative_left = 0;
	int count_positive = 0;
	int count_negative = 0;

	for (size_t i = 0; i < audioData.size(); ++i) {
		if (audioData[i] > 0) {
			mean_positive_left += audioData[i];
			count_positive++;
		}
		else {
			mean_negative_left += audioData[i];
			count_negative++;
		}
	}

	if (count_positive > 0) {
		mean_positive_left /= count_positive;
	}
	if (count_negative > 0) {
		mean_negative_left /= count_negative;
	}

	auto maxElement = std::max_element(audioData.begin(), audioData.end());

	// Определение широких пиков в аудиофайле
	const int peakThreshold = *maxElement * 0.8; // Порог для определения широких пиков

	bool isPeak = false;
	double peakStart = 0.0;
	double peakEnd = 0.0;
	int numSec = wavReaderWriter.GetSF_Info().frames / wavReaderWriter.GetSF_Info().samplerate;

	for (size_t i = 1; i < audioData.size() - 1; ++i) {
		if (std::abs(audioData[i]) > peakThreshold && std::abs(audioData[i]) > std::abs(audioData[i - 1])) {
			if (!isPeak) {
				isPeak = true;
				peakStart = (double)i / audioData.size() * numSec;
			}
		}
		else {
			if (std::abs(audioData[i]) < peakThreshold && std::abs(audioData[i - 1]) > std::abs(audioData[i])) {
				if (isPeak) {
					peakEnd = (double)i / audioData.size() * numSec;
					widePeak.push_back(std::make_pair(peakStart, peakEnd));
					isPeak = false;
				}	
			}
		}
	}

	// Вывод результатов
	std::cout << "Wide peaks found at indices: ";
	for (const auto& peak : widePeak) {
		std::cout << "(" << peak.first << ", " << peak.second << ") ";
	}
	std::cout << std::endl;
	
	/*
	
	// Предполагаем, что широкий пик определяется как пик, который превышает среднее значение на 20%
	double threshold = CalculateAverage() * 1.2;
	auto maxElement = std::max_element(audioData.begin(), audioData.end());
	threshold = *maxElement * 0.7;
	// Ищем пики
	int startIndex = 0;
	for (int i = 1; i < audioData.size(); ++i) {
		if (audioData[i] > threshold && audioData[i - 1] < threshold) {
			startIndex = i;
		}
		else if (audioData[i] < threshold && audioData[i - 1] > threshold) {
			widePeaks.push_back(std::make_pair(startIndex, i - 1));
		}
	}*/

	return widePeaks;
}