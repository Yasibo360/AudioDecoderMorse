#include "MorseСode.h"
#include "MyFunction.h"

MorseСode::MorseСode() : _dot(L"."), _dash(L"-"), _sepSymb(L" "), _sepWord(L"   "),
_dotDuration(100), _dashDuration(3 * _dotDuration), _pauseDuration(_dotDuration),
_wordPauseDuration(7 * _dotDuration)
{
	// Загрузка стандартных словарей
	loadDictionaryFromFile("./Dictionaries/Dictionary Ru.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary En.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary Numeric.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary Service Symbols.txt");

	// Если словарей не найдено, программа не сможет работать
	if (_dictionaries.empty())
	{
		throw "Error: dictionaries not loaded";
	}

	setActiveDictionary(_dictionaries.begin()->first);

	return;
};

MorseСode::~MorseСode() {};

void MorseСode::setDurations(int dot, int dash, int letterPause, int wordPause)
{
	_dotDuration = dot;
	_dashDuration = dash;
	_pauseDuration = letterPause;
	_wordPauseDuration = wordPause;
}
void MorseСode::setActiveDictionary(std::wstring dictionaryName)
{
	std::map<wchar_t, std::wstring> combinedDictionary;
	for (const auto& dict : _dictionaries)
	{
		if (dict.first == dictionaryName || dict.first == L"Цифры" || dict.first == L"Пунктуация") {
			for (const auto& entry : dict.second)
			{
				combinedDictionary[entry.first] = entry.second;
			}
		}	
	}
	_activeDictionary = combinedDictionary;
}

bool MorseСode::loadDictionaryFromFile(const std::string& filename)
{
	std::wifstream file(filename); // Открытие файла для чтения
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>)); // Установка локали для корректной обработки wchar_t

	if (!file.is_open()) {
		std::cerr << "Не удалось открыть файл " << filename << std::endl;
		return false;
	}

	std::wstring line;
	std::wstring alphabetName;

	// Чтение названия алфавита
	if (std::getline(file, alphabetName)) {
		_dictionaries[alphabetName]; // Используем первую строку в качестве ключа для внешней карты

	}
	else {
		std::wcerr << L"Файл пуст." << std::endl;
		return false;
	}

	// Чтение алфавита
	while (std::getline(file, line)) {
		if (line.empty()) return false; // Если пустая строка = алфавит закончен

		// Находим позицию первого пробела или конца строки
		size_t pos = line.find(L' ');
		if (pos == std::wstring::npos) {
			pos = line.size(); // Если пробел не найден, используем длину строки
			return false;
		}

		wchar_t character = line[0]; // Первый символ - это символ Морзе
		std::wstring code = line.substr(2, pos - 2); // Извлекаем код Морзе до первого пробела или конца строки

		// Добавление пары символ-код Морзе в соответствующую внутреннюю карту
		_dictionaries[alphabetName][character] = code;
	}
	file.close();

	return true;
}

std::wstring MorseСode::charToMorse(std::wstring str)
{
	std::wstring morseCode{};

	for (size_t i = 0; i < str.length(); i++)
	{
		str[i] = std::towupper(str[i]);
		if (_activeDictionary.find(str[i]) != _activeDictionary.end())
		{
			morseCode += _activeDictionary.find(str[i])->second + _sepSymb;
		}

		if (str[i] == wchar_t(" "[0]))
		{
			morseCode += _sepWord;
		}
	}

	return morseCode;
};

std::wstring MorseСode::morseToChar(std::wstring str)
{
	std::wstring userStr;
	std::vector<std::wstring> words = Split(str, _sepWord);
	std::vector<std::wstring> symbols;
	bool flag = false;

	for (size_t i = 0; i < words.size(); i++)
	{
		symbols = Split(words[i], _sepSymb);

		for (std::wstring symbol : symbols)
		{
			for (const auto& it : _activeDictionary)
			{
				if (it.second == symbol)
				{
					userStr += it.first;
					flag = true;
				}
			}

			// Обработка неопознанных символов по текущему словарю
			if (!flag)
			{
				userStr += L"#";
			}
		}
		if (i != words.size() - 1) {
			userStr += _sepWord;
		}
	}
	return userStr;
}

std::vector<std::wstring> MorseСode::getAllDictionariesName()
{
	std::vector<std::wstring> dictionariesName;

	for (const auto& dict : _dictionaries)
	{
		dictionariesName.push_back(dict.first);
	}

	return dictionariesName;
}

std::map<wchar_t, std::wstring> MorseСode::getAllDictionariesCombined(std::wstring alphabetName)
{
	std::map<wchar_t, std::wstring> combinedDictionary;
	for (const auto& dict : _dictionaries)
	{
		if (dict.first == alphabetName) {
			for (const auto& entry : dict.second)
			{
				combinedDictionary[entry.first] = entry.second;
			}
		}
	}

	return combinedDictionary;
}

std::wstring MorseСode::peakDurationsToMorse(const std::vector<std::pair<char, float>>& peakDurations, float precision)
{
	std::wstring morseCode;

	float dotDur = std::numeric_limits<float>::infinity();
	float pauseMorseDur = std::numeric_limits<float>::infinity();
	float pauseWordDur = -std::numeric_limits<float>::infinity();

	// Поиск минимального значения символов и максимальной паузы
	for (const auto& duration : peakDurations) {
		if (duration.first == 's') {
			dotDur = std::min(dotDur, duration.second);
		}
		if (duration.first == 'p') {
			pauseWordDur = std::max(pauseWordDur, duration.second);
			pauseMorseDur = std::min(pauseMorseDur, duration.second);
		}
	}

	if (not (std::abs(pauseWordDur - pauseMorseDur * 7) <= std::pow(1.3, precision))) {
		pauseWordDur = 0;
	}

	for (const auto& duration : peakDurations) {
		if (duration.first == 's') {
			if (std::abs(duration.second - dotDur) <= std::pow(4, precision)) {
				morseCode += _dot;
			}
			else if (std::abs(duration.second - dotDur * 3) <= std::pow(4, precision)) {
				morseCode += _dash;
			}
		}
		else if (duration.first == 'p') {
			if (std::abs(duration.second - pauseMorseDur * 3) <= std::pow(3.4, precision)) {
				morseCode += _sepSymb;
			}
			else if (std::abs(duration.second - pauseWordDur) <= std::pow(4, precision) && pauseWordDur != 0) {
				morseCode += _sepWord;
			}
		}
	}

	return morseCode;
}

std::vector<std::pair<float, float>> MorseСode::findWidePeaksInAudioFile(std::string filename)
{
	// Открытие файла для чтения
	SndfileHandle file(filename);

	// Вектор векторов для чтения по каналам
	std::vector<std::vector<float>> samplesByChannels;

	float frames;
	frames = readAudioData(filename, samplesByChannels);

	// Вычисляем пороговое значение
	float max_sample = *std::max_element(samplesByChannels[0].begin(), samplesByChannels[0].end());
	setThreshold(max_sample * 0.5);

	// Вычисляем время для каждого отсчета
	float samplerate = file.samplerate();
	std::vector<float> time(frames);

	for (int i = 0; i < frames; i++) {
		time[i] = i / samplerate;
	}

	// Вычисляем период сигнала
	float period = 1.0 / samplerate * 350;

	std::vector<std::pair<float, float>> widePeaks;
	widePeaks = findWidePeaks(samplesByChannels[0], time, period);

	return widePeaks;
}

std::vector<std::pair<float, float>> MorseСode::findWidePeaks(std::vector<float>& samples, std::vector<float>& time, float period)
{
	// Получаем начальное и конечное время
	float timeStart = time.front();
	float timeEnd = time.back();

	// Ищем индексы пиков, которые выше порога
	std::vector<int> peaks;
	for (int i = 1; i < samples.size() - 1; ++i) {
		if (samples[i] > _threshold) {
			if (samples[i] > samples[i - 1] && samples[i] > samples[i + 1]) {
				peaks.push_back(i);
			}
		}
	}

	// Находим индексы начала и конца широких пиков
	std::vector<std::pair<float, float>> peak_ranges;
	std::vector<std::pair<float, float>> wide_peak_ranges;

	for (int i = 0; i < peaks.size() - 1; ++i) {
		if (time[peaks[i]] >= timeStart && time[peaks[i]] <= timeEnd) {
			if (time[peaks[i + 1]] - time[peaks[i]] <= period) {
				if (peak_ranges.empty()) {
					peak_ranges.emplace_back(time[peaks[i]], time[peaks[i + 1]]);
				}
				else {
					peak_ranges.back().second = time[peaks[i + 1]];
				}
			}
			else {
				if (!peak_ranges.empty()) {
					wide_peak_ranges.push_back(peak_ranges.back());
					peak_ranges.clear();
				}
			}
		}
	}

	// Добавляем последний интервал в wide_peak_ranges
	if (!peak_ranges.empty()) {
		wide_peak_ranges.push_back(peak_ranges.back());
	}

	return wide_peak_ranges;
}

std::vector<std::pair<char, float>> MorseСode::findPeakDurations(std::vector<std::pair<float, float>>& widePeaks, float precision)
{
	std::vector<std::pair<char, float>> peak_durations;
	for (size_t i = 0; i < widePeaks.size(); ++i) {
		float duration = std::round((widePeaks[i].second - widePeaks[i].first) * std::pow(10, precision)) / std::pow(10, precision);
		peak_durations.push_back(std::make_pair('s', duration));

		if (i < widePeaks.size() - 1) {
			float pause = std::round((widePeaks[i + 1].first - widePeaks[i].second) * std::pow(10, precision)) / std::pow(10, precision);
			peak_durations.push_back(std::make_pair('p', pause));
		}
	}

	return peak_durations;
}

void MorseСode::setThreshold(float threshold)
{
	_threshold = threshold;
}

float MorseСode::getThreshold() const
{
	return _threshold;
};

void MorseСode::playMorseCode(std::wstring& morseCode)
{
	std::vector<std::wstring> words = Split(morseCode, _sepWord);
	std::vector<std::wstring> symbols;

	int frequency = 900;

	for (std::wstring word : words)
	{
		symbols = Split(word, _sepSymb);

		for (std::wstring symbol : symbols)
		{
			for (size_t i = 0; i < symbol.length(); i++)
			{
				switch (symbol[i])
				{
				case '.':
					Beep(frequency, _dotDuration);
					break;
				case '-':
					Beep(frequency, _dashDuration*0.8);
					break;
				default:
					break;
				}
				Sleep(_dotDuration);
			}
			Sleep(_dashDuration);
		}
		Sleep(_wordPauseDuration);
	}
	return;
}