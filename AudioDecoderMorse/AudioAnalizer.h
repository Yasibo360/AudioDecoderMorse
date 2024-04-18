#pragma once

#include "WavReaderWriter.h"

class AudioAnalizer
{
private:
    WavReaderWriter wavReaderWriter;
    std::vector<short> audioData;

    

public:
	AudioAnalizer(const std::string& filename = "recorded.wav");
	~AudioAnalizer();

    // Реализация построения графика амплитуды по времени
    void PlotAmplitudeOverTime(HWND hWnd);

    // Реализация построения графика амплитуды по частоте
    void PlotAmplitudeOverFrequency();

    // Реализация поиска пиковых значений
    void FindPeaks();

    // Реализация вычисления среднего значения на промежутке сигнала
    double CalculateAverage();

    // Реализация метода для определения широких пиков
    std::vector<std::pair<int, int>> FindWidePeaks();
};