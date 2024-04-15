#include "AudioRecorder.h"

// ����������� ������ AudioRecorder
AudioRecorder::AudioRecorder(int sampleRate = 44100, int channels = 2, int bitsPerSample = 16, int _sizeBuffer = 4096, const std::string& _filename = "recorded.wav")
	: isRecording(false), countBuffers(4), recordingThread(NULL), recordingThreadId(0) {

	wavReadWrtite.SetSF_Info(sampleRate, channels, SF_FORMAT_WAV | SF_FORMAT_PCM_16);
	WAVEFORMATEX waveFormatX = ConvertSF_InfoToWaveFormatX(wavReadWrtite.GetSF_Info());

	// ������������� sizeBuffer
	sizeBuffer = _sizeBuffer;

	// ������������� ���� ����� �����
	wavReadWrtite.SetFileName(_filename);

	// ������������� �������� ���������� � �������
	waveHeaders.resize(countBuffers);
	buffers.resize(countBuffers);

	// �������� ���������� ������ �����
	MMRESULT result = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveFormatX, (DWORD_PTR)waveInProc, (DWORD_PTR)this, CALLBACK_FUNCTION);
	if (result != MMSYSERR_NOERROR) {
		throw std::runtime_error("������ �������� ���������� ����� �����!");
	}

	// �������� �������� ��� ������������� ������� � �������
	audioQueueMutex = CreateMutex(NULL, FALSE, NULL);
	if (audioQueueMutex == NULL) {
		throw std::runtime_error("������ �������� ��������!");
	}

	// �������� �������� ��� ������������ � ��������� ������
	audioQueueSemaphore = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	if (audioQueueSemaphore == NULL) {
		throw std::runtime_error("������ �������� ��������!");
	}
}

// ���������� ������ AudioRecorder
AudioRecorder::~AudioRecorder() {
	// ���� ������ �������, ���������� ��
	if (isRecording) {
		try {
			StopRecording();
		}
		catch (const std::exception& e) {
			// ����������� ������ ��� �������������
		}
	}

	// ������� ���������� ������ �����
	MMRESULT result = waveInClose(hWaveIn);
	if (result != MMSYSERR_NOERROR) {
		// ����������� ������ ��� �������������
	}

	// ���������� ������, ���������� ��� �������
	for (char* buffer : buffers) {
		delete[] buffer;
	}
	buffers.clear(); // �������� ������ �������
	waveHeaders.clear();

	// ������� ������� � �������
	if (audioQueueMutex != NULL) {
		CloseHandle(audioQueueMutex);
		audioQueueMutex = NULL; // �������� ���������� ��������
	}

	if (audioQueueSemaphore != NULL) {
		CloseHandle(audioQueueSemaphore);
		audioQueueSemaphore = NULL; // �������� ���������� ��������
	}

	// ������� ����� ������, ���� �� ��� ������
	if (recordingThread != NULL) {
		WaitForSingleObject(recordingThread, INFINITE);
		CloseHandle(recordingThread);
		recordingThread = NULL; // �������� ���������� ������
	}
}

bool AudioRecorder::IsRecording() const {
	return isRecording;
};

// ����� ��� ������ ������ �����
void AudioRecorder::StartRecording() {
	// ��������, �� ���� �� ������ ������
	if (isRecording) {
		throw std::runtime_error("��� ������������!");
	}
	
	// ��������� ���� ��� ������ ������
	wavReadWrtite.OpenFileForWrite(wavReadWrtite.GetFileName());

	// ��������� ������ � ���������� ���������� �������
	for (int i = 0; i < countBuffers; ++i) {
		PrepareAudioBuffer(hWaveIn, waveHeaders[i], sizeBuffer);
	}

	// ������ ������
	waveInStart(hWaveIn);
	isRecording = true;

	// ������ ������ ������
	recordingThread = CreateThread(NULL, 0, recordingThreadProc, this, 0, &recordingThreadId);
}

// ����� ��� ��������� ������ �����
void AudioRecorder::StopRecording() {
	if (!isRecording) {
		return;
	}

	// ������ ������ ������ � ������������� ����������
	isRecording = false;
	ReleaseSemaphore(audioQueueSemaphore, 1, NULL);

	// �������� ���������� ������ ������
	WaitForSingleObject(recordingThread, INFINITE);

	// ������������ �������
	for (const auto& header : waveHeaders) {
		waveInUnprepareHeader(hWaveIn, (LPWAVEHDR)&header, sizeof(WAVEHDR));
	}

	for (const auto& buffer : buffers) {
		delete[] buffer;
	}

	// ��������� � �������� ���������� ������ �����
	MMRESULT result = waveInStop(hWaveIn);
	if (result != MMSYSERR_NOERROR) {
		// ��������� ������
	}
	
	// �������� ����� ��� ������
	wavReadWrtite.CloseFile();
}

// ������� ��� ���������� ������ ��� ������ �����
void AudioRecorder::PrepareAudioBuffer(HWAVEIN hWaveIn, WAVEHDR& waveHdr, int sizeBuffer) {
	// ��������� ������ ��� ������
	char* buffer = new char[sizeBuffer]; // ������ ������ ������ ���� ������� wfx.nBlockAlign
	waveHdr.lpData = (LPSTR)buffer;
	if (waveHdr.lpData == NULL) {
		return; // �� ������� �������� ������
	}

	// ���������� ��������� WAVEHDR
	waveHdr.dwBufferLength = sizeBuffer * sizeof(char);
	waveHdr.dwBytesRecorded = 0;
	waveHdr.dwUser = 0;
	waveHdr.dwFlags = 0;
	waveHdr.dwLoops = 0;
	waveHdr.lpNext = 0;
	waveHdr.reserved = 0;

	// ���������� ������
	MMRESULT result = waveInPrepareHeader(hWaveIn, &waveHdr, sizeof(WAVEHDR));
	if (result != MMSYSERR_NOERROR) {
		delete[] waveHdr.lpData; // ������������ ���������� ������
		return; // ������ ��� ���������� ������
	}

	// ���������� ������ � ������� ������
	result = waveInAddBuffer(hWaveIn, &waveHdr, sizeof(WAVEHDR));
	if (result != MMSYSERR_NOERROR) {
		waveInUnprepareHeader(hWaveIn, &waveHdr, sizeof(WAVEHDR)); // ������ ���������� ������
		delete[] waveHdr.lpData; // ������������ ���������� ������
		return; // ������ ��� ���������� ������ � �������
	}

	return; // ����� ������� �����������
}

WAVEFORMATEX AudioRecorder::ConvertSF_InfoToWaveFormatX(const SF_INFO& sfInfo) {
	WAVEFORMATEX waveFormatX{};
	
	waveFormatX.wFormatTag = WAVE_FORMAT_PCM; // �� ��������� PCM ������
	waveFormatX.nSamplesPerSec = sfInfo.samplerate;
	waveFormatX.wBitsPerSample = sizeof(char) * 8;
	waveFormatX.nChannels = sfInfo.channels;
	waveFormatX.cbSize = 0; // �� ��������� 0
	waveFormatX.nBlockAlign = (waveFormatX.nChannels * waveFormatX.wBitsPerSample) / 8;
	waveFormatX.nAvgBytesPerSec = waveFormatX.nSamplesPerSec * waveFormatX.nBlockAlign;

	return waveFormatX;
}

// ����������� ���������� �����, ������������ ��� ������ �����
void CALLBACK AudioRecorder::waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	if (uMsg == WIM_DATA) {
		AudioRecorder* recorder = reinterpret_cast<AudioRecorder*>(dwInstance);
		WAVEHDR* waveHdr = reinterpret_cast<WAVEHDR*>(dwParam1);

		if (waveHdr->dwBytesRecorded > 0) {
			WaitForSingleObject(recorder->audioQueueMutex, INFINITE);
			recorder->audioQueue.push(std::vector<char>(waveHdr->lpData, waveHdr->lpData + waveHdr->dwBytesRecorded));
			ReleaseMutex(recorder->audioQueueMutex);
			ReleaseSemaphore(recorder->audioQueueSemaphore, 1, NULL); // ������ � ��������� ������
		}

		// ���������� � ���������� ������ ������� � �������
		recorder->PrepareAudioBuffer(recorder->hWaveIn, *waveHdr, waveHdr->dwBufferLength);
	}
}

// ����������� �����, ������������ ��� ������ ����� � ����
DWORD WINAPI AudioRecorder::recordingThreadProc(LPVOID lpParam) {
	AudioRecorder* recorder = reinterpret_cast<AudioRecorder*>(lpParam);
	while (recorder->isRecording) {
		// �������� ��������� ������ � �������
		WaitForSingleObject(recorder->audioQueueSemaphore, INFINITE);
		if (!recorder->isRecording) {
			break; // ����� �� ������, ���� ������ �����������
		}

		// ��������� �������� ������ �� �������
		WaitForSingleObject(recorder->audioQueueMutex, INFINITE);
		std::vector<char> audioData = recorder->audioQueue.front();
		
		
		std::vector<short> data;
		for (char c : audioData) {
			short s = (c); // �������������� char � short
			data.push_back(s); // ���������� �������� � ������ short
		}




		recorder->audioQueue.pop();
		ReleaseMutex(recorder->audioQueueMutex);

		// ������ �������� ������ � �������� ����
		//recorder->wavReadWrtite.WriteData(reinterpret_cast<short*>(audioData.data()), audioData.size());
		//recorder->wavReadWrtite.WriteData(&audioData[0], audioData.size());
		recorder->wavReadWrtite.WriteData(&data[0], audioData.size());
	}
	return 0;
}