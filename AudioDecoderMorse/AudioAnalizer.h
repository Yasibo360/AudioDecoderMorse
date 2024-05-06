#pragma once

#include "WavReaderWriter.h"
#include "Plot_AmpTime.h"
#include "MyFunction.h"

class AudioAnalizer
{
private:
    WavReaderWriter _wavReaderWriter;
    std::vector<short> _audioData;
    //Plot_AmpTime _plot;  

public:
	AudioAnalizer();
	~AudioAnalizer();

    bool readDataFromFile(const std::string& filename = "recorded.wav");

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