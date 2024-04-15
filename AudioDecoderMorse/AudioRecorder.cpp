#include "AudioRecorder.h"

// Конструктор класса AudioRecorder
AudioRecorder::AudioRecorder(int sampleRate = 44100, int channels = 2, int bitsPerSample = 16, int _sizeBuffer = 4096, const std::string& _filename = "recorded.wav")
	: isRecording(false), countBuffers(4), recordingThread(NULL), recordingThreadId(0) {

	wavReadWrtite.SetSF_Info(sampleRate, channels, SF_FORMAT_WAV | SF_FORMAT_PCM_16);
	WAVEFORMATEX waveFormatX = ConvertSF_InfoToWaveFormatX(wavReadWrtite.GetSF_Info());

	// Инициализация sizeBuffer
	sizeBuffer = _sizeBuffer;

	// Инициализация поля имени файла
	wavReadWrtite.SetFileName(_filename);

	// Инициализация векторов заголовков и буферов
	waveHeaders.resize(countBuffers);
	buffers.resize(countBuffers);

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

	// Освободить память, выделенную для буферов
	for (char* buffer : buffers) {
		delete[] buffer;
	}
	buffers.clear(); // Очистить вектор буферов
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
		PrepareAudioBuffer(hWaveIn, waveHeaders[i], sizeBuffer);
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

	// Сигнал потоку записи о необходимости завершения
	isRecording = false;
	ReleaseSemaphore(audioQueueSemaphore, 1, NULL);

	// Ожидание завершения потока записи
	WaitForSingleObject(recordingThread, INFINITE);

	// Освобождение буферов
	for (const auto& header : waveHeaders) {
		waveInUnprepareHeader(hWaveIn, (LPWAVEHDR)&header, sizeof(WAVEHDR));
	}

	for (const auto& buffer : buffers) {
		delete[] buffer;
	}

	// Остановка и закрытие устройства записи звука
	MMRESULT result = waveInStop(hWaveIn);
	if (result != MMSYSERR_NOERROR) {
		// Обработка ошибки
	}
	
	// Закрытие файла для записи
	wavReadWrtite.CloseFile();
}

// Функция для подготовки буфера для записи звука
void AudioRecorder::PrepareAudioBuffer(HWAVEIN hWaveIn, WAVEHDR& waveHdr, int sizeBuffer) {
	// Выделение памяти для буфера
	char* buffer = new char[sizeBuffer]; // Размер буфера должен быть кратным wfx.nBlockAlign
	waveHdr.lpData = (LPSTR)buffer;
	if (waveHdr.lpData == NULL) {
		return; // Не удалось выделить память
	}

	// Заполнение структуры WAVEHDR
	waveHdr.dwBufferLength = sizeBuffer * sizeof(char);
	waveHdr.dwBytesRecorded = 0;
	waveHdr.dwUser = 0;
	waveHdr.dwFlags = 0;
	waveHdr.dwLoops = 0;
	waveHdr.lpNext = 0;
	waveHdr.reserved = 0;

	// Подготовка буфера
	MMRESULT result = waveInPrepareHeader(hWaveIn, &waveHdr, sizeof(WAVEHDR));
	if (result != MMSYSERR_NOERROR) {
		delete[] waveHdr.lpData; // Освобождение выделенной памяти
		return; // Ошибка при подготовке буфера
	}

	// Добавление буфера в очередь записи
	result = waveInAddBuffer(hWaveIn, &waveHdr, sizeof(WAVEHDR));
	if (result != MMSYSERR_NOERROR) {
		waveInUnprepareHeader(hWaveIn, &waveHdr, sizeof(WAVEHDR)); // Отмена подготовки буфера
		delete[] waveHdr.lpData; // Освобождение выделенной памяти
		return; // Ошибка при добавлении буфера в очередь
	}

	return; // Буфер успешно подготовлен
}

WAVEFORMATEX AudioRecorder::ConvertSF_InfoToWaveFormatX(const SF_INFO& sfInfo) {
	WAVEFORMATEX waveFormatX{};
	
	waveFormatX.wFormatTag = WAVE_FORMAT_PCM; // По умолчанию PCM формат
	waveFormatX.nSamplesPerSec = sfInfo.samplerate;
	waveFormatX.wBitsPerSample = sizeof(char) * 8;
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

		if (waveHdr->dwBytesRecorded > 0) {
			WaitForSingleObject(recorder->audioQueueMutex, INFINITE);
			recorder->audioQueue.push(std::vector<char>(waveHdr->lpData, waveHdr->lpData + waveHdr->dwBytesRecorded));
			ReleaseMutex(recorder->audioQueueMutex);
			ReleaseSemaphore(recorder->audioQueueSemaphore, 1, NULL); // Сигнал о доступных данных
		}

		// Подготовка и добавление буфера обратно в очередь
		recorder->PrepareAudioBuffer(recorder->hWaveIn, *waveHdr, waveHdr->dwBufferLength);
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
		std::vector<char> audioData = recorder->audioQueue.front();
		
		
		std::vector<short> data;
		for (char c : audioData) {
			short s = (c); // Преобразование char в short
			data.push_back(s); // Добавление элемента в вектор short
		}




		recorder->audioQueue.pop();
		ReleaseMutex(recorder->audioQueueMutex);

		// Запись звуковых данных в выходной файл
		//recorder->wavReadWrtite.WriteData(reinterpret_cast<short*>(audioData.data()), audioData.size());
		//recorder->wavReadWrtite.WriteData(&audioData[0], audioData.size());
		recorder->wavReadWrtite.WriteData(&data[0], audioData.size());
	}
	return 0;
}