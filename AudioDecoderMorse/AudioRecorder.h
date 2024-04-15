#pragma once

#include "WavReaderWriter.h"

class AudioRecorder {
private:
    // Переменная для работы с устройством записи
    HWAVEIN hWaveIn;

    // Переменные для синхронизации потоков
    bool isRecording;
    std::queue<std::vector<char>> audioQueue;
    HANDLE audioQueueMutex;
    HANDLE audioQueueSemaphore;

    // Переменные для работы с потоком
    HANDLE recordingThread;
    DWORD recordingThreadId;

    // Переменные для работы с буферами
    int sizeBuffer;
    short countBuffers;
    std::vector<WAVEHDR> waveHeaders;
    std::vector<char*> buffers;

    static void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    static DWORD WINAPI recordingThreadProc(LPVOID lpParam);

public:
    // Переменная для работы с файлом WAV
    WavReaderWriter wavReadWrtite;

    AudioRecorder(int sampleRate, int channels, int bitsPerSample, int _sizeBuffer, const std::string& filename);
    ~AudioRecorder();

    bool IsRecording() const;
    void StartRecording();
    void StopRecording();
    void PrepareAudioBuffer(HWAVEIN hWaveIn, WAVEHDR& waveHdr, int sizeBuffer);
    WAVEFORMATEX ConvertSF_InfoToWaveFormatX(const SF_INFO& sfInfo);
};