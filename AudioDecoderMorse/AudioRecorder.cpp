#include "AudioRecorder.h"

// ����������� ������ AudioRecorder
AudioRecorder::AudioRecorder(int sampleRate = 44100, int numChannels = 2, int bitsPerSample = 16, int _sizeBuffer = 4096, const std::string& _filename = "recorded.wav")
	: isRecording(false), countBuffers(4), recordingThread(NULL), recordingThreadId(0) {

	// ������������� waveFormatX � ��������� �����������
	writer.wavHeader.fmt.waveFormatX.wFormatTag = WAVE_FORMAT_PCM;
	writer.wavHeader.fmt.waveFormatX.nSamplesPerSec = sampleRate;
	writer.wavHeader.fmt.waveFormatX.wBitsPerSample = bitsPerSample;
	writer.wavHeader.fmt.waveFormatX.nChannels = numChannels;
	writer.wavHeader.fmt.waveFormatX.cbSize = 0;
	writer.wavHeader.fmt.waveFormatX.nBlockAlign = (numChannels * bitsPerSample) / 8;
	writer.wavHeader.fmt.waveFormatX.nAvgBytesPerSec = sampleRate * writer.wavHeader.fmt.waveFormatX.nBlockAlign;

	// ������������� sizeBuffer
	sizeBuffer = _sizeBuffer;
	if (sizeBuffer % writer.wavHeader.fmt.waveFormatX.nBlockAlign != 0) {
		// ���� ������ ������ �� ������ writer.wavHeader.fmt.waveFormatX.nBlockAlign, ����������� ��� �� ���������� ��������
		sizeBuffer += writer.wavHeader.fmt.waveFormatX.nBlockAlign - (sizeBuffer % writer.wavHeader.fmt.waveFormatX.nBlockAlign);
	}

	// ������������� ���� ����� �����
	writer.fileName = _filename;

	// ������������� �������� ���������� � �������
	waveHeaders.resize(countBuffers);
	buffers.resize(countBuffers);

	// �������� ���������� ������ �����
	MMRESULT result = waveInOpen(&hWaveIn, WAVE_MAPPER, &writer.wavHeader.fmt.waveFormatX, (DWORD_PTR)waveInProc, (DWORD_PTR)this, CALLBACK_FUNCTION);
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

bool AudioRecorder::IsRecording()
{
	return isRecording;
};

// ����� ��� ������ ������ �����
void AudioRecorder::StartRecording() {
	// ��������, �� ���� �� ������ ������
	if (isRecording) {
		throw std::runtime_error("��� ������������!");
	}

	// ��������� ���� ��� ������ ������
	writer.outFile.open(writer.fileName, std::ios::binary);
	if (!writer.outFile.is_open()) {
		throw std::runtime_error("������ �������� ����� ��� ������!");
	}

	// ���������� ���������
	writer.outFile.seekp(44, std::ios::beg);

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

	// ������� ������ ����� ������
	uint32_t sizeDataFile;
	sizeDataFile = writer.GetSizeFile();

	// ���������� ���������� ����� � ��������� ��������� ����� WAV
	writer.wavHeader.riff.chunkSize = sizeDataFile
		- sizeof(writer.wavHeader.riff.chunkID)
		- sizeof(writer.wavHeader.riff.format);

	writer.wavHeader.data.subChunk2Size = sizeDataFile
		- sizeof(writer.wavHeader.riff)
		- sizeof(writer.wavHeader.fmt.subchunk1Id)
		- sizeof(writer.wavHeader.fmt.subchunk1Size)
		- sizeof(writer.wavHeader.fmt.waveFormatX)
		+ sizeof(writer.wavHeader.fmt.waveFormatX.cbSize)
		- sizeof(writer.wavHeader.data.subChunk2ID)
		- sizeof(writer.wavHeader.data.subChunk2Size);

	// ������ ��������� WAV �����
	writer.WriteHeader(writer.wavHeader);

	// �������� ����� ��� ������
	writer.outFile.close();
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
	waveHdr.dwBufferLength = sizeBuffer;
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
		recorder->audioQueue.pop();
		ReleaseMutex(recorder->audioQueueMutex);

		// ������ �������� ������ � �������� ����
		recorder->writer.outFile.write(audioData.data(), audioData.size());
	}
	return 0;
}