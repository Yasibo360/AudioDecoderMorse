#include "AudioRecorder.h"

// Конструктор класса AudioRecorder
AudioRecorder::AudioRecorder(int sampleRate = 44100, int numChannels = 2, int bitsPerSample = 16, int _sizeBuffer = 4096, const std::string& _filename = "recorded.wav")
	: isRecording(false), countBuffers(4), recordingThread(NULL), recordingThreadId(0) {

	// Инициализация waveFormatX с заданными параметрами
	writer.wavHeader.fmt.waveFormatX.wFormatTag = WAVE_FORMAT_PCM;
	writer.wavHeader.fmt.waveFormatX.nSamplesPerSec = sampleRate;
	writer.wavHeader.fmt.waveFormatX.wBitsPerSample = bitsPerSample;
	writer.wavHeader.fmt.waveFormatX.nChannels = numChannels;
	writer.wavHeader.fmt.waveFormatX.cbSize = 0;
	writer.wavHeader.fmt.waveFormatX.nBlockAlign = (numChannels * bitsPerSample) / 8;
	writer.wavHeader.fmt.waveFormatX.nAvgBytesPerSec = sampleRate * writer.wavHeader.fmt.waveFormatX.nBlockAlign;

	// Инициализация sizeBuffer
	sizeBuffer = _sizeBuffer;
	if (sizeBuffer % writer.wavHeader.fmt.waveFormatX.nBlockAlign != 0) {
		// Если размер буфера не кратен writer.wavHeader.fmt.waveFormatX.nBlockAlign, увеличиваем его до ближайшего кратного
		sizeBuffer += writer.wavHeader.fmt.waveFormatX.nBlockAlign - (sizeBuffer % writer.wavHeader.fmt.waveFormatX.nBlockAlign);
	}

	// Инициализация поля имени файла
	writer.fileName = _filename;

	// Инициализация векторов заголовков и буферов
	waveHeaders.resize(countBuffers);
	buffers.resize(countBuffers);

	// Открытие устройства записи звука
	MMRESULT result = waveInOpen(&hWaveIn, WAVE_MAPPER, &writer.wavHeader.fmt.waveFormatX, (DWORD_PTR)waveInProc, (DWORD_PTR)this, CALLBACK_FUNCTION);
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

bool AudioRecorder::IsRecording()
{
	return isRecording;
};

// Метод для начала записи звука
void AudioRecorder::StartRecording() {
	// Проверка, не идет ли сейчас запись
	if (isRecording) {
		throw std::runtime_error("Уже записывается!");
	}

	// Открываем файл для записи данных
	writer.outFile.open(writer.fileName, std::ios::binary);
	if (!writer.outFile.is_open()) {
		throw std::runtime_error("Ошибка открытия файла для записи!");
	}

	// Пропускаем заголовок
	writer.outFile.seekp(44, std::ios::beg);

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

	// Считаем размер блока данных
	uint32_t sizeDataFile;
	sizeDataFile = writer.GetSizeFile();

	// Заполнение оставшихся полей в структуре заголовка файла WAV
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

	// Запись заголовка WAV файла
	writer.WriteHeader(writer.wavHeader);

	// Закрытие файла для записи
	writer.outFile.close();
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
	waveHdr.dwBufferLength = sizeBuffer;
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
		recorder->audioQueue.pop();
		ReleaseMutex(recorder->audioQueueMutex);

		// Запись звуковых данных в выходной файл
		recorder->writer.outFile.write(audioData.data(), audioData.size());
	}
	return 0;
}