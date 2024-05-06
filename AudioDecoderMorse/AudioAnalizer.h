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

    // ���������� ���������� ������� ��������� �� �������
    void PlotAmplitudeOverTime(HWND hWnd);

    // ���������� ���������� ������� ��������� �� �������
    void PlotAmplitudeOverFrequency();

    // ���������� ������ ������� ��������
    void FindPeaks();

    // ���������� ���������� �������� �������� �� ���������� �������
    double CalculateAverage();

    // ���������� ������ ��� ����������� ������� �����
    std::vector<std::pair<int, int>> FindWidePeaks();
};