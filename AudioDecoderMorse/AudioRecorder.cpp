#include "AudioRecorder.h"

// Конструктор класса AudioRecorder
AudioRecorder::AudioRecorder(const std::string& filename, int sampleRate, int channels, int format)
	: _filename(filename), _isRecording(false), _countBuffers(4), _recordingThread(NULL), _recordingThreadId(0) {

	_waveFormatX.wFormatTag = format; // По умолчанию PCM формат
	_waveFormatX.nSamplesPerSec = sampleRate;
	_waveFormatX.wBitsPerSample = sizeof(short) * 8;
	_waveFormatX.nChannels = channels;
	_waveFormatX.cbSize = 0; // По умолчанию 0
	_waveFormatX.nBlockAlign = (_waveFormatX.nChannels * _waveFormatX.wBitsPerSample) / 8;
	_waveFormatX.nAvgBytesPerSec = _waveFormatX.nSamplesPerSec * _waveFormatX.nBlockAlign;

	// Инициализация sizeBuffer
	_sizeBuffer = _waveFormatX.nSamplesPerSec * _waveFormatX.nBlockAlign;

	// Инициализация векторов заголовков и буферов
	_waveHeaders.resize(_countBuffers);
	_audioBuffers.resize(_countBuffers);

	// Открытие устройства записи звука
	MMRESULT result = waveInOpen(&_hWaveIn, WAVE_MAPPER, &_waveFormatX, (DWORD_PTR)waveInProc, (DWORD_PTR)this, CALLBACK_FUNCTION);
	if (result != MMSYSERR_NOERROR) {
		//throw std::runtime_error("Ошибка открытия устройства ввода звука!");
		MessageBox(NULL, L"Не найдено подходящего устройства ввода. \n\
							Убедитесь, что в вашей системе есть устройства ввода и запустите приложение заново!", L"Ошибка", MB_OK | MB_ICONERROR);
		exit(0);
	}

	// Создание мьютекса для синхронизации доступа к очереди
	_audioQueueMutex = CreateMutex(NULL, FALSE, NULL);
	if (_audioQueueMutex == NULL) {
		throw std::runtime_error("Ошибка создания мьютекса!");
	}

	// Создание семафора для сигнализации о доступных данных
	_audioQueueSemaphore = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	if (_audioQueueSemaphore == NULL) {
		throw std::runtime_error("Ошибка создания семафора!");
	}
}

// Деструктор класса AudioRecorder
AudioRecorder::~AudioRecorder() {
	// Если запись активна, остановить ее
	if (_isRecording) {
		try {
			StopRecording();
		}
		catch (const std::exception& e) {
			// Логирование ошибки или игнорирование
		}
	}

	// Закрыть устройство записи звука
	MMRESULT result = waveInClose(_hWaveIn);
	if (result != MMSYSERR_NOERROR) {
		// Логирование ошибки или игнорирование
	}
	
	for (auto& buffer : _audioBuffers) {
		buffer.clear();
	}

	_audioBuffers.clear(); // Очистить вектор буферов
	_waveHeaders.clear();

	// Закрыть мьютекс и семафор
	if (_audioQueueMutex != NULL) {
		CloseHandle(_audioQueueMutex);
		_audioQueueMutex = NULL; // Сбросить дескриптор мьютекса
	}

	if (_audioQueueSemaphore != NULL) {
		CloseHandle(_audioQueueSemaphore);
		_audioQueueSemaphore = NULL; // Сбросить дескриптор семафора
	}

	// Закрыть поток записи, если он был создан
	if (_recordingThread != NULL) {
		WaitForSingleObject(_recordingThread, INFINITE);
		CloseHandle(_recordingThread);
		_recordingThread = NULL; // Сбросить дескриптор потока
	}
}

bool AudioRecorder::IsRecording() const {
	return _isRecording;
};

// Метод для начала записи звука
void AudioRecorder::StartRecording() {
	// Проверка, не идет ли сейчас запись
	if (_isRecording) {
		throw std::runtime_error("Уже записывается!");
	}

	// Открытие файла на перезапись
	SndfileHandle file(_filename, SFM_WRITE);

	// Выделение памяти и подготовка заголовков буферов
	for (int i = 0; i < _countBuffers; ++i) {
		PrepareAudioBuffer(_hWaveIn, _waveHeaders[i], _audioBuffers[i], _sizeBuffer);
	}

	// Начало записи
	waveInStart(_hWaveIn);
	_isRecording = true;

	// Запуск потока записи
	_recordingThread = CreateThread(NULL, 0, recordingThreadProc, this, 0, &_recordingThreadId);
}

// Метод для остановки записи звука
void AudioRecorder::StopRecording() {
	if (!_isRecording) {
		return;
	}

	// Остановка и закрытие устройства записи звука
	MMRESULT result = waveInStop(_hWaveIn);
	if (result != MMSYSERR_NOERROR) {
		// Обработка ошибки
	}

	// Сигнал потоку записи о необходимости завершения
	_isRecording = false;
	ReleaseSemaphore(_audioQueueSemaphore, 1, NULL);

	// Ожидание завершения потока записи
	WaitForSingleObject(_recordingThread, INFINITE);


	// Освобождение буферов
	for (const auto& header : _waveHeaders) {
		waveInUnprepareHeader(_hWaveIn, (LPWAVEHDR)&header, sizeof(WAVEHDR));
	}

	for (auto& buffer : _audioBuffers) {
		buffer.clear();
	}
	
	// Закрыть поток записи, если он был создан
	if (_recordingThread != NULL) {
		WaitForSingleObject(_recordingThread, INFINITE);
		CloseHandle(_recordingThread);
		_recordingThread = NULL; // Сбросить дескриптор потока
	}
}

// Функция для подготовки буфера для записи звука
void AudioRecorder::PrepareAudioBuffer(HWAVEIN hWaveIn, WAVEHDR& waveHdr, std::vector<short>& audioBuffer, int sizebuffer) {
	// Выделение памяти для буфера
	audioBuffer.clear();
	audioBuffer.resize(sizebuffer);

	// Заполнение структуры WAVEHDR
	waveHdr.lpData = reinterpret_cast<LPSTR>(audioBuffer.data());
	waveHdr.dwBufferLength = sizebuffer; // Размер буфера должен быть кратным wfx.nBlockAlign
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

// Статический обработчик звука, используемый для записи звука
void CALLBACK AudioRecorder::waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	if (uMsg == WIM_DATA) {
		AudioRecorder* recorder = reinterpret_cast<AudioRecorder*>(dwInstance);
		WAVEHDR* waveHdr = reinterpret_cast<WAVEHDR*>(dwParam1);
		std::vector<short>* audioData = reinterpret_cast<std::vector<short>*>(waveHdr->dwUser);

		if (waveHdr->dwBytesRecorded > 0) {
			WaitForSingleObject(recorder->_audioQueueMutex, INFINITE);
			recorder->_audioQueue.push(*audioData);
			ReleaseMutex(recorder->_audioQueueMutex);
			ReleaseSemaphore(recorder->_audioQueueSemaphore, 1, NULL); // Сигнал о доступных данных
		}

		// Подготовка и добавление буфера обратно в очередь
		recorder->PrepareAudioBuffer(recorder->_hWaveIn, *waveHdr, *audioData, waveHdr->dwBufferLength);
	}
}

// Статический метод, используемый для записи звука в файл
DWORD WINAPI AudioRecorder::recordingThreadProc(LPVOID lpParam) {
	AudioRecorder* recorder = reinterpret_cast<AudioRecorder*>(lpParam);
	while (recorder->_isRecording) {
		// Ожидание доступных данных в очереди
		WaitForSingleObject(recorder->_audioQueueSemaphore, INFINITE);
		if (!recorder->_isRecording) {
			break; // Выход из потока, если запись остановлена
		}

		// Получение звуковых данных из очереди
		WaitForSingleObject(recorder->_audioQueueMutex, INFINITE);
		
		std::vector<short> audioData;
		if (!recorder->_audioQueue.empty()) {
			audioData = recorder->_audioQueue.front();
			recorder->_audioQueue.pop();

			// Запись звуковых данных в выходной файл
			SndfileHandle file(
				recorder->_filename, 
				SFM_RDWR, // Открываем файл для дозаписи данных
				SF_FORMAT_WAV | SF_FORMAT_PCM_16, 
				recorder->_waveFormatX.nChannels,
				recorder->_waveFormatX.nSamplesPerSec
			);
			// Почему-то устройство заполняет данными только 1/2 буфера
			file.write(audioData.data(), audioData.size() / 2);
		}				
		ReleaseMutex(recorder->_audioQueueMutex);
	}
	return 0;
}