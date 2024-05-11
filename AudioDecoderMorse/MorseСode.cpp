#include "MorseСode.h"
#include "MyFunction.h"

MorseСode::MorseСode() : _dot(L"."), _dash(L"-"), _sepSymb(L" "), _sepWord(L"   "),
_dotDuration(100), _dashDuration(3 * _dotDuration), _pauseDuration(_dotDuration),
_wordPauseDuration(7 * _dotDuration)
{
	// Загрузка стандартных словарей
	loadDictionaryFromFile("./Dictionaries/Dictionary Numeric.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary Service Symbols.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary En.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary Ru.txt");

	// Если словарей не найдено, программа не сможет работать
	if (_dictionaries.empty())
	{
		throw "Error: dictionaries not loaded";
	}
	return;
};

MorseСode::~MorseСode() {};

void MorseСode::setDurations(int dot, int dash, int letterPause, int wordPause)
{
	_dotDuration = dot;
	_dashDuration = dash;
	_pauseDuration = letterPause;
	_wordPauseDuration = wordPause;
};

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

	// Проверяем каждую букву во всех словарях
	for (size_t i = 0; i < str.length(); i++)
	{
		str[i] = std::towupper(str[i]);
		for (const auto& dictionary : _dictionaries) {
			if (dictionary.second.find(str[i]) != dictionary.second.end())
			{
				morseCode += dictionary.second.find(str[i])->second + _sepSymb;
				break;
			}

			/*if (Dictionary.second.begin()->first <= str[i] && str[i] <= Dictionary.second.rbegin()->first)
			{
				morseCode += Dictionary.second.find(str[i])->second + MorseСode::sepSymb;
				break;
			}*/
			if (str[i] == wchar_t(" "[0]))
			{
				morseCode += _sepWord;
				break;
			}
		}
	}
	//if (morseCode.empty()) throw L"Не найдено совпадений";

	return morseCode;
};

std::wstring MorseСode::morseToChar(std::wstring str)
{
    std::wstring userStr;
    std::vector<std::wstring> words = Split(str, _sepWord);
    std::vector<std::wstring> symbols;
    bool flag = false;

    for (std::wstring word : words)
    {
        symbols = Split(word, _sepSymb);

        for (std::wstring symbol : symbols)
        {
            for (const auto& dictEntry : _dictionaries)
            {
                const std::map<wchar_t, std::wstring>& currentDictionary = dictEntry.second;

				for (const auto& it : currentDictionary)
				{
					if (it.second == symbol)
					{
						userStr += it.first;
						flag = true;
						break;
					}
				}
            }

            // Обработка неопознанных символов по текущему словарю
            if (!flag)
            {
                userStr += L"#";
            }
        }
        userStr += _sepWord;
    }
    return userStr;
}

std::map<wchar_t, std::wstring> MorseСode::getAllDictionariesCombined()
{
    std::map<wchar_t, std::wstring> combinedDictionary;
    for (const auto& dict : _dictionaries)
    {
        for (const auto& entry : dict.second)
        {
            combinedDictionary[entry.first] = entry.second;
        }
    }
    return combinedDictionary;
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

std::wstring MorseСode::peakDurationsToMorse(const std::vector<std::pair<char, float>>& peakDurations, float precision)
{
	std::wstring morseCode;

	float minNumber = std::numeric_limits<float>::infinity();
	float maxPause = -std::numeric_limits<float>::infinity();

	// Поиск минимального значения символов и максимальной паузы
	for (const auto& duration : peakDurations) {
		if (duration.first == 's') {
			minNumber = std::min(minNumber, duration.second);
		}
		if (duration.first == 'p') {
			maxPause = std::max(maxPause, duration.second);
		}
	}

	for (const auto& duration : peakDurations) {
		if (duration.first == 's') {
			if (std::abs(duration.second - minNumber) <= std::pow(10, precision)) {
				morseCode += L".";
			}
			else if (std::abs(duration.second - minNumber * 3) <= std::pow(10, precision)) {
				morseCode += L"-";
			}
		}
		else if (duration.first == 'p') {
			if (std::abs(duration.second - maxPause) <= std::pow(10, precision + 1)) {
				morseCode += L" ";
			}
		}
	}

	return morseCode;
}

std::wstring MorseСode::audioFileToMorse(std::string filename)
{
	// Открытие файла для чтения
	SndfileHandle file(filename);

	// Вектор векторов для чтения по каналам
	std::vector<std::vector<float>> samplesByChannels;

	float frames;
	frames = readAudioData(filename, samplesByChannels);

	// Вычисляем пороговое значение
	float max_sample = *std::max_element(samplesByChannels[0].begin(), samplesByChannels[0].end());
	_threshold = max_sample * 0.5;

	// Вычисляем время для каждого отсчета
	float samplerate = file.samplerate();
	std::vector<float> time(frames);

	for (int i = 0; i < frames; i++) {
		time[i] = i / samplerate;
	}

	// Вычисляем период сигнала
	float period = 1.0 / samplerate;

	std::vector<std::pair<float, float>> widePeaks;
	widePeaks = findWidePeaks(samplesByChannels[0], time, period);

	std::vector<std::pair<char, float>> peakDurations;
	peakDurations = findPeakDurations(widePeaks, 2);

	std::wstring result = peakDurationsToMorse(peakDurations);

	return result;
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
			if (time[peaks[i + 1]] - time[peaks[i]] <= 200 * period) {
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

float MorseСode::getThreshold()
{
	return _threshold;
};

void MorseСode::playMorseCode(std::wstring& morseCode)
{
	std::vector<std::wstring> words = Split(morseCode, _sepWord);
	std::vector<std::wstring> symbols;

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
					Beep(900, _dotDuration);
					break;
				case '-':
					Beep(900, _dashDuration);
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
};