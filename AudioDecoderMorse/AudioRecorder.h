#pragma once

#include <iostream>
#include <windows.h>
#include <queue>
#include <vector>
#include <sndfile.h>
#include <sndfile.hh>

class AudioRecorder {
public:
  // ���������� ��� ������ � ������ WAV 
  SndfileHandle file;

  // ����������� ������ AudioRecorder
  AudioRecorder(const std::string& filename = "recorded.wav", int sampleRate = 44100, int channels = 2, int format = WAVE_FORMAT_PCM);
  // ���������� ������ AudioRecorder
  ~AudioRecorder();

  // ����� ��� �������� ���� �� ������
  bool IsRecording() const;
  // ����� ��� ������ ������
  void StartRecording();
  // ����� ��� ��������� ������
  void StopRecording();

  // ����� ��� ���������� ����� ������
  void PrepareAudioBuffer(HWAVEIN hWaveIn, WAVEHDR& waveHdr, std::vector<short>& audioBuffer, int _sizebuffer);

private:
  // ���������� ��� ������ � ����������� ������
  HWAVEIN _hWaveIn;

  // ���������� ��� ������������� �������
  bool _isRecording;
  std::queue<std::vector<short>> _audioQueue;
  HANDLE _audioQueueMutex;
  HANDLE _audioQueueSemaphore;

  // ���������� ��� ������ � �������
  HANDLE _recordingThread;
  DWORD _recordingThreadId;

  // ���������� ��� ������ � ��������
  int _sizeBuffer;
  short _countBuffers;
  std::vector<WAVEHDR> _waveHeaders;
  std::vector<std::vector<short>> _audioBuffers;

  std::string _filename;
  WAVEFORMATEX _waveFormatX;

  // ����������� ����� ��������� ������ ��� ��������� �������� ������
  static void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
  // ����������� ����� ������ ������
  static DWORD WINAPI recordingThreadProc(LPVOID lpParam);
};