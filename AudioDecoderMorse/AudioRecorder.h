#pragma once

#include <iostream>
#include <windows.h>
#include <queue>
#include <vector>
#include <sndfile.h>
#include <sndfile.hh>

class AudioRecorder {
public:
  // Переменная для работы с файлом WAV 
  SndfileHandle file;

  // Конструктор класса AudioRecorder
  AudioRecorder(const std::string& filename = "recorded.wav", int sampleRate = 44100, int channels = 2, int format = WAVE_FORMAT_PCM);
  // Деструктор класса AudioRecorder
  ~AudioRecorder();

  // Метод для проверки идет ли запись
  bool IsRecording() const;
  // Метод для начала записи
  void StartRecording();
  // Метод для остановки записи
  void StopRecording();

  // Метод для подготовки аудио буфера
  void PrepareAudioBuffer(HWAVEIN hWaveIn, WAVEHDR& waveHdr, std::vector<short>& audioBuffer, int _sizebuffer);

private:
  // Переменная для работы с устройством записи
  HWAVEIN _hWaveIn;

  // Переменные для синхронизации потоков
  bool _isRecording;
  std::queue<std::vector<short>> _audioQueue;
  HANDLE _audioQueueMutex;
  HANDLE _audioQueueSemaphore;

  // Переменные для работы с потоком
  HANDLE _recordingThread;
  DWORD _recordingThreadId;

  // Переменные для работы с буферами
  int _sizeBuffer;
  short _countBuffers;
  std::vector<WAVEHDR> _waveHeaders;
  std::vector<std::vector<short>> _audioBuffers;

  std::string _filename;
  WAVEFORMATEX _waveFormatX;

  // Статический метод обратного вызова для обработки входящих звуков
  static void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
  // Статический метод потока записи
  static DWORD WINAPI recordingThreadProc(LPVOID lpParam);
};