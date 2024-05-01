#include "AudioAnalizer.h"
#include "AudioGraph.h"

AudioAnalizer::AudioAnalizer(const std::string& filename) {
	// ������ ����� WAV
	wavReaderWriter.OpenFileForRead(filename);

	int bitsPerSample = sizeof(short) * 8;

	// ������������� sizeBuffer
	int sizeBuffer = wavReaderWriter.GetSF_Info().samplerate * wavReaderWriter.GetSF_Info().channels * bitsPerSample / 8;

	audioData.resize(sizeBuffer);
	wavReaderWriter.ReadData(audioData.data(), audioData.size());

	wavReaderWriter.CloseFile();
}

AudioAnalizer::~AudioAnalizer() {}

// ���������� ���������� ������� ��������� �� �������
void AudioAnalizer::PlotAmplitudeOverTime(HWND hWnd) {
	int samplerate = wavReaderWriter.GetSF_Info().samplerate;
	int channels = wavReaderWriter.GetSF_Info().channels;

	// �������������� HWND � sf::RenderWindow*
	sf::WindowHandle handle = reinterpret_cast<sf::WindowHandle>(hWnd);
	sf::RenderWindow window(handle);

	// ��������� ����� ���� ����
	window.clear(sf::Color(8, 111, 161)); //brushPane

	unsigned int width = window.getSize().x;
	unsigned int height = window.getSize().y;

	// ������������ ������� ������������� ������� � ���������
	float marginX = width * 0.05f; // ������ ����� � ������ 5% �� ������
	float marginY = height * 0.2f; // ������ ������ � �����, ����� ���������

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
		// ��������� �������
		sf::Event event;
		while (window1.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window1.close();
		}

		// ���������
		window1.clear();
		window1.draw(audioGraph);
		window1.display();

		// ���������� ������������
		audioGraph.update();
	}



	/*

	// ������� �������������, ������� ���������� ������� ���������
	sf::FloatRect drawArea(sf::Vector2f(marginX, marginY), sf::Vector2f(width - 2 * marginX, height - 2 * marginY));

	// ������� ������ ������ ����� ������ ������� �������
	sf::RectangleShape borderRect(sf::Vector2f(drawArea.getSize().x + 1, drawArea.getSize().y + 1));
	borderRect.setPosition(drawArea.getPosition().x - 1, drawArea.getPosition().y - 1);
	borderRect.setFillColor(sf::Color::Transparent);
	borderRect.setFillColor(sf::Color::White);
	borderRect.setOutlineColor(sf::Color::Black);
	borderRect.setOutlineThickness(1.0f);

	// ������� ������������ �������� ���������
	float maxAmplitude = *std::max_element(audioData.begin(), audioData.end());
	float normalize = (drawArea.getSize().y / 2) / maxAmplitude;

	// ������� ������ ����������� ��������� �� �������
	sf::VertexArray graph(sf::LinesStrip);
	for (int i = 0; i < audioData.size(); ++i) {
		float x = i * (drawArea.getSize().x / audioData.size()); // ������������ ����� �� ��� X
		float y = audioData[i] * normalize; // ����������� �������� ���������
		y = std::min(y, drawArea.getSize().y); // ������������ �������� �� ������ ������� ���������
		y += drawArea.getPosition().y + drawArea.getSize().y / 2; // �������� �� ������� ������� ���������
		graph.append(sf::Vertex(sf::Vector2f(drawArea.getPosition().x + x, y), sf::Color::Red)); // ���� �������
	}

	// ��������� �������� �������
	sf::Text title("������ ��������� �� �������", sf::Font(), 20);
	title.setFillColor(sf::Color::Black);
	title.setPosition(drawArea.getPosition().x + drawArea.getSize().x / 2,
		drawArea.getPosition().y - drawArea.getSize().y / 10); // ������� ��� ������

	// ������� ����� ������ �����
	sf::VertexArray grid(sf::Lines);
	grid.resize(2 * (drawArea.getSize().x + drawArea.getSize().y)); // ��������� ������ ���������� ������
	grid.setPrimitiveType(sf::Lines);
	grid[0].position = sf::Vector2f(drawArea.getPosition().x, drawArea.getPosition().y);
	grid[1].position = sf::Vector2f(drawArea.getPosition().x + drawArea.getSize().x, drawArea.getPosition().y);
	grid[2].position = sf::Vector2f(drawArea.getPosition().x, drawArea.getPosition().y);
	grid[3].position = sf::Vector2f(drawArea.getPosition().x, drawArea.getPosition().y + drawArea.getSize().y);

	// ������ ����� ���� � �������������
	for (int i = 0; i < grid.getVertexCount(); ++i) {
		grid[i].color = sf::Color(200, 200, 200, 150);
	}

	// ��������� ������� ��� �����
	// ��� ������������ ����� �����
	for (int i = 0; i <= 10; ++i) {
		sf::Text label(std::to_string(i), sf::Font(), 12);
		label.setFillColor(sf::Color::Black);
		label.setPosition(drawArea.getPosition().x - 10, drawArea.getPosition().y + i * drawArea.getSize().y / 10);
	}

	// ��� �������������� ����� �����
	for (int i = 0; i <= audioData.size(); i += audioData.size() / 10) {
		sf::Text label(std::to_string(i), sf::Font(), 12);
		label.setFillColor(sf::Color::Black);
		label.setPosition(drawArea.getPosition().x + i * drawArea.getSize().x / audioData.size(), drawArea.getPosition().y + drawArea.getSize().y + 10);
	}

	// ��������� ������� ����
	sf::Text xAxisLabel("�����", sf::Font(), 12);
	xAxisLabel.setFillColor(sf::Color::Black);
	xAxisLabel.setPosition(drawArea.getPosition().x + drawArea.getSize().x - 30, drawArea.getPosition().y + drawArea.getSize().y + 10);

	sf::Text yAxisLabel("���������", sf::Font(), 12);
	yAxisLabel.setFillColor(sf::Color::Black);
	yAxisLabel.setPosition(drawArea.getPosition().x - 50, drawArea.getPosition().y + drawArea.getSize().y / 2);

	// ��������� ������� � ����
	window.draw(borderRect); // �������� ������������� � ����
	window.draw(grid);
	//window.draw(title);
	//window.draw(xAxisLabel);
	//window.draw(yAxisLabel);
	window.draw(graph);
	

	// ����������� ������������� �� ������
	window.display();*/
}

// ���������� ���������� ������� ��������� �� �������
void AudioAnalizer::PlotAmplitudeOverFrequency() {

}

// ���������� ������ ������� ��������
void AudioAnalizer::FindPeaks() {

}

// ���������� ���������� �������� �������� �� ���������� �������
double AudioAnalizer::CalculateAverage() {
	double average_val = 0;
	for (const auto& data : audioData) {
		average_val += data;
	}
	average_val = average_val / audioData.size();
	return average_val;
}

// ���������� ������ ��� ����������� ������� �����
std::vector<std::pair<int, int>> AudioAnalizer::FindWidePeaks() {
	std::vector<std::pair<int, int>> widePeaks;
	std::vector<std::pair<double, double>> widePeak;
	// ��������� ������� �������� ��� ������������� � ������������� ������ ���������
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

	// ����������� ������� ����� � ����������
	const int peakThreshold = *maxElement * 0.8; // ����� ��� ����������� ������� �����

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

	// ����� �����������
	std::cout << "Wide peaks found at indices: ";
	for (const auto& peak : widePeak) {
		std::cout << "(" << peak.first << ", " << peak.second << ") ";
	}
	std::cout << std::endl;
	
	/*
	
	// ������������, ��� ������� ��� ������������ ��� ���, ������� ��������� ������� �������� �� 20%
	double threshold = CalculateAverage() * 1.2;
	auto maxElement = std::max_element(audioData.begin(), audioData.end());
	threshold = *maxElement * 0.7;
	// ���� ����
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