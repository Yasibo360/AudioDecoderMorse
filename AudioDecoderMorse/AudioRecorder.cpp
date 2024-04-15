#include "AudioRecorder.h"

// Конструктор класса AudioRecorder
AudioRecorder::AudioRecorder(int sampleRate = 44100, int channels = 2, int _bitsPerSample = 16, int _sizeBuffer = 4096, const std::string& _filename = "recorded.wav")
	: isRecording(false), countBuffers(4), recordingThread(NULL), recordingThreadId(0) {

	wavReadWrtite.SetSF_Info(sampleRate, channels, SF_FORMAT_WAV | SF_FORMAT_PCM_16);
	WAVEFORMATEX waveFormatX = ConvertSF_InfoToWaveFormatX(wavReadWrtite.GetSF_Info());

	int bitsPerSample = sizeof(short) * 8;

	// Инициализация sizeBuffer
	//sizeBuffer = _sizeBuffer;
	sizeBuffer = wavReadWrtite.GetSF_Info().samplerate * wavReadWrtite.GetSF_Info().channels * bitsPerSample / 8;


	// Инициализация поля имени файла
	wavReadWrtite.SetFileName(_filename);

	// Инициализация векторов заголовков и буферов
	waveHeaders.resize(countBuffers);
	audioBuffers.resize(countBuffers);

	// Открытие устройства записи звука
	MMRESULT result = waveInOpen(&hWaveIn, WAVE_MAPPER, &waveFormatX, (DWORD_PTR)waveInProc, (DWORD_PTR)this, CALLBACK_FUNCTION);
	if (result != MMSYSERR_NOERROR) {
		throw std::runtime_error("Ошибка открытия устройства ввода звука!");
	}

	// Создание мьютекса для синхронизации доступа к очереди
	audioQueueMutex = CreateMutex(NULL, FALSE, NULL);
	if (audioQueueMutex == NULL) {
		throw std::runtime_error("Ошибка создания мьютекса!");
	}

	// Создание семафора для сигнализации о доступных данных
	audioQueueSemaphore = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	if (audioQueueSemaphore == NULL) {
		throw std::runtime_error("Ошибка создания семафора!");
	}
}

// Деструктор класса AudioRecorder
AudioRecorder::~AudioRecorder() {
	// Если запись активна, остановить ее
	if (isRecording) {
		try {
			StopRecording();
		}
		catch (const std::exception& e) {
			// Логирование ошибки или игнорирование
		}
	}

	// Закрыть устройство записи звука
	MMRESULT result = waveInClose(hWaveIn);
	if (result != MMSYSERR_NOERROR) {
		// Логирование ошибки или игнорирование
	}
	
	for (auto& buffer : audioBuffers) {
		buffer.clear();
	}
	
	/*
	// Освободить память, выделенную для буферов
	for (const auto& buffer : buffers) {
		delete[] buffer;
	}
	*/
	audioBuffers.clear(); // Очистить вектор буферов
	waveHeaders.clear();

	// Закрыть мьютекс и семафор
	if (audioQueueMutex != NULL) {
		CloseHandle(audioQueueMutex);
		audioQueueMutex = NULL; // Сбросить дескриптор мьютекса
	}

	if (audioQueueSemaphore != NULL) {
		CloseHandle(audioQueueSemaphore);
		audioQueueSemaphore = NULL; // Сбросить дескриптор семафора
	}

	// Закрыть поток записи, если он был создан
	if (recordingThread != NULL) {
		WaitForSingleObject(recordingThread, INFINITE);
		CloseHandle(recordingThread);
		recordingThread = NULL; // Сбросить дескриптор потока
	}
}

bool AudioRecorder::IsRecording() const {
	return isRecording;
};

// Метод для начала записи звука
void AudioRecorder::StartRecording() {
	// Проверка, не идет ли сейчас запись
	if (isRecording) {
		throw std::runtime_error("Уже записывается!");
	}
	
	// Открываем файл для записи данных
	wavReadWrtite.OpenFileForWrite(wavReadWrtite.GetFileName());

	// Выделение памяти и подготовка заголовков буферов
	for (int i = 0; i < countBuffers; ++i) {
		PrepareAudioBuffer(hWaveIn, waveHeaders[i], audioBuffers[i], sizeBuffer);
	}

	// Начало записи
	waveInStart(hWaveIn);
	isRecording = true;

	// Запуск потока записи
	recordingThread = CreateThread(NULL, 0, recordingThreadProc, this, 0, &recordingThreadId);
}

// Метод для остановки записи звука
void AudioRecorder::StopRecording() {
	if (!isRecording) {
		return;
	}

	// Остановка и закрытие устройства записи звука
	MMRESULT result = waveInStop(hWaveIn);
	if (result != MMSYSERR_NOERROR) {
		// Обработка ошибки
	}

	// Сигнал потоку записи о необходимости завершения
	isRecording = false;
	ReleaseSemaphore(audioQueueSemaphore, 1, NULL);

	// Ожидание завершения потока записи
	WaitForSingleObject(recordingThread, INFINITE);


	// Освобождение буферов
	for (const auto& header : waveHeaders) {
		waveInUnprepareHeader(hWaveIn, (LPWAVEHDR)&header, sizeof(WAVEHDR));
	}

	for (auto& buffer : audioBuffers) {
		buffer.clear();
	}

	/*
	for (const auto& buffer : buffers) {
		delete[] buffer;
	}
	*/
	
	
	// Закрыть поток записи, если он был создан
	if (recordingThread != NULL) {
		WaitForSingleObject(recordingThread, INFINITE);
		CloseHandle(recordingThread);
		recordingThread = NULL; // Сбросить дескриптор потока
	}

	// Закрытие файла для записи
	wavReadWrtite.CloseFile();
}

// Функция для подготовки буфера для записи звука
void AudioRecorder::PrepareAudioBuffer(HWAVEIN hWaveIn, WAVEHDR& waveHdr, std::vector<short>& audioBuffer, int _sizebuffer) {
	// Выделение памяти для буфера
	audioBuffer.clear();
	audioBuffer.resize(_sizebuffer);

	// Заполнение структуры WAVEHDR
	waveHdr.lpData = reinterpret_cast<LPSTR>(audioBuffer.data());
	waveHdr.dwBufferLength = _sizebuffer; // Размер буфера должен быть кратным wfx.nBlockAlign
	waveHdr.dwBytesRecorded = 0;
	waveHdr.dwUser = reinterpret_cast<DWORD_PTR>(&audioBuffer);
	waveHdr.dwFlags = 0;
	waveHdr.dwLoops = 0;
	waveHdr.lpNext = nullptr;
	waveHdr.reserved = 0;

	// Подготовка буфера
	MMRESULT result = waveInPrepareHeader(hWaveIn, &waveHdr, sizeof(WAVEHDR));
	if (result != MMSYSERR_NOERROR) {
		//delete[] buffer; // Освобождение выделенной памяти
		return; // Ошибка при подготовке буфера
	}

	// Добавление буфера в очередь записи
	result = waveInAddBuffer(hWaveIn, &waveHdr, sizeof(WAVEHDR));
	if (result != MMSYSERR_NOERROR) {
		waveInUnprepareHeader(hWaveIn, &waveHdr, sizeof(WAVEHDR)); // Отмена подготовки буфера
		//delete[] buffer; // Освобождение выделенной памяти
		return; // Ошибка при добавлении буфера в очередь
	}

	return; // Буфер успешно подготовлен
}

WAVEFORMATEX AudioRecorder::ConvertSF_InfoToWaveFormatX(const SF_INFO& sfInfo) {
	WAVEFORMATEX waveFormatX{};
	
	waveFormatX.wFormatTag = WAVE_FORMAT_PCM; // По умолчанию PCM формат
	waveFormatX.nSamplesPerSec = sfInfo.samplerate;
	waveFormatX.wBitsPerSample = sizeof(short) * 8;
	waveFormatX.nChannels = sfInfo.channels;
	waveFormatX.cbSize = 0; // По умолчанию 0
	waveFormatX.nBlockAlign = (waveFormatX.nChannels * waveFormatX.wBitsPerSample) / 8;
	waveFormatX.nAvgBytesPerSec = waveFormatX.nSamplesPerSec * waveFormatX.nBlockAlign;

	return waveFormatX;
}

// Статический обработчик звука, используемый для записи звука
void CALLBACK AudioRecorder::waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	if (uMsg == WIM_DATA) {
		AudioRecorder* recorder = reinterpret_cast<AudioRecorder*>(dwInstance);
		WAVEHDR* waveHdr = reinterpret_cast<WAVEHDR*>(dwParam1);
		std::vector<short>* audioData = reinterpret_cast<std::vector<short>*>(waveHdr->dwUser);

		if (waveHdr->dwBytesRecorded > 0) {
			WaitForSingleObject(recorder->audioQueueMutex, INFINITE);
			recorder->audioQueue.push(*audioData);
			ReleaseMutex(recorder->audioQueueMutex);
			ReleaseSemaphore(recorder->audioQueueSemaphore, 1, NULL); // Сигнал о доступных данных
		}

		// Подготовка и добавление буфера обратно в очередь
		recorder->PrepareAudioBuffer(recorder->hWaveIn, *waveHdr, *audioData, waveHdr->dwBufferLength);
	}
}

// Статический метод, используемый для записи звука в файл
DWORD WINAPI AudioRecorder::recordingThreadProc(LPVOID lpParam) {
	AudioRecorder* recorder = reinterpret_cast<AudioRecorder*>(lpParam);
	while (recorder->isRecording) {
		// Ожидание доступных данных в очереди
		WaitForSingleObject(recorder->audioQueueSemaphore, INFINITE);
		if (!recorder->isRecording) {
			break; // Выход из потока, если запись остановлена
		}

		// Получение звуковых данных из очереди
		WaitForSingleObject(recorder->audioQueueMutex, INFINITE);
		
		std::vector<short> audioData;
		if (!recorder->audioQueue.empty()) {
			audioData = recorder->audioQueue.front();
			recorder->audioQueue.pop();

			// Запись звуковых данных в выходной файл		
			//recorder->wavReadWrtite.WriteData(audioData.data(), recorder->wavReadWrtite.GetSF_Info().samplerate * recorder->wavReadWrtite.GetSF_Info().channels);
			recorder->wavReadWrtite.WriteData(audioData.data(), audioData.size() / 2);
		}				
		ReleaseMutex(recorder->audioQueueMutex);		
	}
	return 0;
}