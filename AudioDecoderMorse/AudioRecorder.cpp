#include "AudioRecorder.h"

// ����������� ������ AudioRecorder
AudioRecorder::AudioRecorder(int sampleRate = 44100, int channels = 2, int bitsPerSample = 16, int _sizeBuffer = 4096, const std::string& _filename = "recorded.wav")
	: isRecording(false), countBuffers(4), recordingThread(NULL), recordingThreadId(0) {

	wavReadWrtite.SetSF_Info(sampleRate, channels, SF_FORMAT_WAV | SF_FORMAT_PCM_16);
	WAVEFORMATEX waveFormatX = ConvertSF_InfoToWaveFormatX(wavReadWrtite.GetSF_Info());

	// ������������� sizeBuffer
	//sizeBuffer = _sizeBuffer;
	sizeBuffer = wavReadWrtite.GetSF_Info().samplerate * wavReadWrtite.GetSF_Info().channels * (sizeof(short) * 8) / 8;

	// ������������� ���� ����� �����
	wavReadWrtite.SetFileName(_filename);

	// ������������� �������� ���������� � �������
	waveHeaders.resize(countBuffers);
	audioBuffers.resize(countBuffers);

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
	/*
	// ���������� ������, ���������� ��� �������
	for (const auto& buffer : buffers) {
		delete[] buffer;
	}
	*/
	audioBuffers.clear(); // �������� ������ �������
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
		PrepareAudioBuffer(hWaveIn, waveHeaders[i], audioBuffers[i], sizeBuffer);
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
	/*
	for (const auto& buffer : buffers) {
		delete[] buffer;
	}
	*/
	// ��������� � �������� ���������� ������ �����
	MMRESULT result = waveInStop(hWaveIn);
	if (result != MMSYSERR_NOERROR) {
		// ��������� ������
	}
	
	// �������� ����� ��� ������
	wavReadWrtite.CloseFile();
}

// ������� ��� ���������� ������ ��� ������ �����
void AudioRecorder::PrepareAudioBuffer(HWAVEIN hWaveIn, WAVEHDR& waveHdr, std::vector<char>& audioBuffer, int _sizebuffer) {
	// ��������� ������ ��� ������
	audioBuffer.resize(_sizebuffer);
	// ���������� ��������� WAVEHDR
	waveHdr.lpData = reinterpret_cast<LPSTR>(audioBuffer.data());
	waveHdr.dwBufferLength = _sizebuffer; // ������ ������ ������ ���� ������� wfx.nBlockAlign
	waveHdr.dwBytesRecorded = 0;
	waveHdr.dwUser = reinterpret_cast<DWORD_PTR>(&audioBuffer);
	waveHdr.dwFlags = 0;
	waveHdr.dwLoops = 0;
	waveHdr.lpNext = nullptr;
	waveHdr.reserved = 0;

	// ���������� ������
	MMRESULT result = waveInPrepareHeader(hWaveIn, &waveHdr, sizeof(WAVEHDR));
	if (result != MMSYSERR_NOERROR) {
		//delete[] buffer; // ������������ ���������� ������
		return; // ������ ��� ���������� ������
	}

	// ���������� ������ � ������� ������
	result = waveInAddBuffer(hWaveIn, &waveHdr, sizeof(WAVEHDR));
	if (result != MMSYSERR_NOERROR) {
		waveInUnprepareHeader(hWaveIn, &waveHdr, sizeof(WAVEHDR)); // ������ ���������� ������
		//delete[] buffer; // ������������ ���������� ������
		return; // ������ ��� ���������� ������ � �������
	}

	return; // ����� ������� �����������
}

WAVEFORMATEX AudioRecorder::ConvertSF_InfoToWaveFormatX(const SF_INFO& sfInfo) {
	WAVEFORMATEX waveFormatX{};
	
	waveFormatX.wFormatTag = WAVE_FORMAT_PCM; // �� ��������� PCM ������
	waveFormatX.nSamplesPerSec = sfInfo.samplerate;
	waveFormatX.wBitsPerSample = sizeof(short) * 8;
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
		std::vector<char>* audioData = reinterpret_cast<std::vector<char>*>(waveHdr->dwUser);

		if (waveHdr->dwBytesRecorded > 0) {
			WaitForSingleObject(recorder->audioQueueMutex, INFINITE);
			//recorder->audioQueue.push(std::vector<short>(waveHdr->lpData, waveHdr->lpData + waveHdr->dwBytesRecorded));
			recorder->audioQueue.push(*audioData);
			ReleaseMutex(recorder->audioQueueMutex);
			ReleaseSemaphore(recorder->audioQueueSemaphore, 1, NULL); // ������ � ��������� ������
		}

		// ���������� � ���������� ������ ������� � �������
		recorder->PrepareAudioBuffer(recorder->hWaveIn, *waveHdr, *audioData, waveHdr->dwBufferLength);
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
		


		/*
		std::vector<short> shortData;
		shortData.reserve(audioData.size() / sizeof(short));
		for (size_t i = 0; i < audioData.size(); i += 2) {
			short value = static_cast<short>((audioData[i + 1] << 8) | audioData[i]);
			shortData.push_back(value);
		}
		*/

		recorder->audioQueue.pop();
		ReleaseMutex(recorder->audioQueueMutex);

		// ������ �������� ������ � �������� ����
		recorder->wavReadWrtite.WriteData(reinterpret_cast<short*>(audioData.data()), audioData.size());
		//recorder->wavReadWrtite.WriteData(audioData.data(), audioData.size());
		//recorder->wavReadWrtite.WriteData(data.data(), audioData.size());

		//sf_write_raw(recorder->wavReadWrtite.file, shortData.data(), shortData.size());
		//sf_writef_short(recorder->wavReadWrtite.file, shortData.data(), shortData.size());
	}
	return 0;
}