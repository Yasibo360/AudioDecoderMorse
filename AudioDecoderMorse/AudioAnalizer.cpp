#include "AudioAnalizer.h"

AudioAnalizer::AudioAnalizer() {}

AudioAnalizer::~AudioAnalizer() {}

bool AudioAnalizer::readDataFromFile(const std::string& filename) {
	// ������ ����� WAV
	_wavReaderWriter.OpenFileForRead(filename);

	int bitsPerSample = sizeof(short) * 8;

	// ������������� sizeBuffer
	int sizeBuffer = _wavReaderWriter.GetSF_Info().samplerate * _wavReaderWriter.GetSF_Info().channels * bitsPerSample / 8;

	_audioData.resize(sizeBuffer);
	_wavReaderWriter.ReadData(_audioData.data(), _audioData.size());

	_wavReaderWriter.CloseFile();

	return true;
}

// ���������� ���������� ������� ��������� �� �������
void AudioAnalizer::PlotAmplitudeOverTime(HWND hWnd) {
	if (_audioData.empty()) {
		readDataFromFile();
	}
	
	int samplerate = _wavReaderWriter.GetSF_Info().samplerate;
	int channels = _wavReaderWriter.GetSF_Info().channels;

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
	if (_audioData.size() % 2 == 0)
	{
		for (size_t i = 0; i < _audioData.size(); i += 2)
		{
			left_channel.push_back(_audioData[i]);
		}
	}

	//std::string audioFile = "recorded.wav";
	//Plot_AmpTime plot(sf::Vector2f(800, 600), sf::Vector2f(0, 0), audioFile, "arial.ttf");
	//plot.addCurve("My Curve", graphAmplitudeFromTime, time, dat);

	//while (window.isOpen()) {
	//	sf::Event event;
	//	while (window.pollEvent(event)) {
	//		if (event.type == sf::Event::Closed)
	//			window.close();
	//	}
	//	window.clear(sf::Color(200, 200, 200));
	//	window.draw(plot);

	//	window.display();
	//}


	//std::string audioFile = "recorded.wav";
	//AudioGraph audioGraph(audioData, sf::Vector2i(0, 0));


	//sf::RenderWindow window1(sf::VideoMode(800, 600), "Audio Graph");
	//window1.setFramerateLimit(60);


	//while (window1.isOpen())
	//{
	//	// ��������� �������
	//	sf::Event event;
	//	while (window1.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//			window1.close();
	//	}

	//	// ���������
	//	window1.clear();
	//	window1.draw(audioGraph);
	//	window1.display();

	//	// ���������� ������������
	//	audioGraph.update();
	//}
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
	for (const auto& data : _audioData) {
		average_val += data;
	}
	average_val = average_val / _audioData.size();
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

	for (size_t i = 0; i < _audioData.size(); ++i) {
		if (_audioData[i] > 0) {
			mean_positive_left += _audioData[i];
			count_positive++;
		}
		else {
			mean_negative_left += _audioData[i];
			count_negative++;
		}
	}

	if (count_positive > 0) {
		mean_positive_left /= count_positive;
	}
	if (count_negative > 0) {
		mean_negative_left /= count_negative;
	}

	auto maxElement = std::max_element(_audioData.begin(), _audioData.end());

	// ����������� ������� ����� � ����������
	const int peakThreshold = *maxElement * 0.8; // ����� ��� ����������� ������� �����

	bool isPeak = false;
	double peakStart = 0.0;
	double peakEnd = 0.0;
	int numSec = _wavReaderWriter.GetSF_Info().frames / _wavReaderWriter.GetSF_Info().samplerate;

	for (size_t i = 1; i < _audioData.size() - 1; ++i) {
		if (std::abs(_audioData[i]) > peakThreshold && std::abs(_audioData[i]) > std::abs(_audioData[i - 1])) {
			if (!isPeak) {
				isPeak = true;
				peakStart = (double)i / _audioData.size() * numSec;
			}
		}
		else {
			if (std::abs(_audioData[i]) < peakThreshold && std::abs(_audioData[i - 1]) > std::abs(_audioData[i])) {
				if (isPeak) {
					peakEnd = (double)i / _audioData.size() * numSec;
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