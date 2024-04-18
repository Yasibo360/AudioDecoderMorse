#pragma once

#include "WavReaderWriter.h"

class AudioRecorder {
private:
    // Переменная для работы с устройством записи
    HWAVEIN hWaveIn;

    // Переменные для синхронизации потоков
    bool isRecording;
    std::queue<std::vector<short>> audioQueue;
    HANDLE audioQueueMutex;
    HANDLE audioQueueSemaphore;

    // Переменные для работы с потоком
    HANDLE recordingThread;
    DWORD recordingThreadId;

    // Переменные для работы с буферами
    int sizeBuffer;
    short countBuffers;
    std::vector<WAVEHDR> waveHeaders;
    std::vector<std::vector<short>> audioBuffers;

    static void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    static DWORD WINAPI recordingThreadProc(LPVOID lpParam);

public:
    // Переменная для работы с файлом WAV
    WavReaderWriter wavReadWrtite;

    AudioRecorder(const std::string& filename = "recorded.wav", int sampleRate = 44100, int channels = 2, int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16);
    ~AudioRecorder();

    bool IsRecording() const;
    void StartRecording();
    void StopRecording();
    void PrepareAudioBuffer(HWAVEIN hWaveIn, WAVEHDR& waveHdr, std::vector<short>& audioBuffer, int _sizebuffer);
    WAVEFORMATEX ConvertSF_InfoToWaveFormatX(const SF_INFO& sfInfo);
};