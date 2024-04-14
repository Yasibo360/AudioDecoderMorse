#pragma once

#include "WavFileWriter.h"

class AudioRecorder {
private:
    // ���������� ��� ������ � ����������� ������
    HWAVEIN hWaveIn;

    // ���������� ��� ������������� �������
    bool isRecording;
    std::queue<std::vector<char>> audioQueue;
    HANDLE audioQueueMutex;
    HANDLE audioQueueSemaphore;

    // ���������� ��� ������ � �������
    HANDLE recordingThread;
    DWORD recordingThreadId;

    // ���������� ��� ������ � ��������
    int sizeBuffer;
    short countBuffers;
    std::vector<WAVEHDR> waveHeaders;
    std::vector<char*> buffers;

    static void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    static DWORD WINAPI recordingThreadProc(LPVOID lpParam);

public:
    // ���������� ��� ������ � ������ WAV
    WavFileWriter writer;

    AudioRecorder();
    AudioRecorder(int sampleRate, int numChannels, int bitsPerSample, int _sizeBuffer, const std::string& filename);
    ~AudioRecorder();

    bool IsRecording();
    void StartRecording();
    void StopRecording();
    void PrepareAudioBuffer(HWAVEIN hWaveIn, WAVEHDR& waveHdr, int sizeBuffer);
};