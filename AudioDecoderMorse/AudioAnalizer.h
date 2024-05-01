#pragma once

#include "WavReaderWriter.h"
#include "Plot_AmpTime.h"

class AudioAnalizer
{
private:
    WavReaderWriter wavReaderWriter;
    std::vector<short> audioData;

    

public:
	AudioAnalizer(const std::string& filename = "recorded.wav");
	~AudioAnalizer();

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