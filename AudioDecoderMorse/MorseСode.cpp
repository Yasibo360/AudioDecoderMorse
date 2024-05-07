#include "MorseСode.h"
#include "TextOperations.h"

#include <fstream>
#include <iostream>
#include <codecvt>
#include <cwctype>


MorseСode::MorseСode() : dot(L"."), dash(L"-"), sepSymb(L" "), sepWord(L"   "),
dotDuration(100), dashDuration(3 * dotDuration), pauseDuration(dotDuration),
wordPauseDuration(7 * dotDuration)
{
	// Загрузка стандартных словарей
	loadDictionaryFromFile("./Dictionaries/Dictionary Numeric.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary Service Symbols.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary En.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary Ru.txt");

	// Если словарей не найдено, программа не сможет работать
	if (dictionaries.empty())
	{
		return;
	}
	else {
		// Устанавливаем текущий словарь - Кириллица
		setCurrentDictionary(dictionaries.begin()->first);
	}
	return;
};

MorseСode::~MorseСode() {};

void MorseСode::setCurrentDictionary(const std::wstring& dictionatyName)
{
	currentDictionary = dictionatyName;
	return;
};

void MorseСode::setDurations(int dot, int dash, int letterPause, int wordPause)
{
	dotDuration = dot;
	dashDuration = dash;
	pauseDuration = letterPause;
	wordPauseDuration = wordPause;
};

void MorseСode::loadDictionaryFromFile(const std::string& filename)
{
	std::wifstream file(filename); // Открытие файла для чтения
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>)); // Установка локали для корректной обработки wchar_t

	if (!file.is_open()) {
		std::cerr << "Не удалось открыть файл " << filename << std::endl;
		return;
	}

	std::wstring line;
	std::wstring alphabetName;

	// Чтение названия алфавита
	if (std::getline(file, alphabetName)) {
		dictionaries[alphabetName]; // Используем первую строку в качестве ключа для внешней карты

	}
	else {
		std::wcerr << L"Файл пуст." << std::endl;
		return;
	}

	// Чтение алфавита
	while (std::getline(file, line)) {
		if (line.empty()) return; // Если пустая строка = алфавит закончен

		// Находим позицию первого пробела или конца строки
		size_t pos = line.find(L' ');
		if (pos == std::wstring::npos) {
			pos = line.size(); // Если пробел не найден, используем длину строки
			return;
		}

		wchar_t character = line[0]; // Первый символ - это символ Морзе
		std::wstring code = line.substr(2, pos - 2); // Извлекаем код Морзе до первого пробела или конца строки

		// Добавление пары символ-код Морзе в соответствующую внутреннюю карту
		dictionaries[alphabetName][character] = code;
	}
	file.close();

	return;
}

std::wstring MorseСode::CharToMorse(std::wstring str)
{
	std::wstring morseCode{};

	// Проверяем каждую букву во всех словарях
	for (size_t i = 0; i < str.length(); i++)
	{
		str[i] = std::towupper(str[i]);
		for (const auto& dictionary : dictionaries) {
			if (dictionary.second.find(str[i]) != dictionary.second.end())
			{
				morseCode += dictionary.second.find(str[i])->second + MorseСode::sepSymb;
				break;
			}

			/*if (Dictionary.second.begin()->first <= str[i] && str[i] <= Dictionary.second.rbegin()->first)
			{
				morseCode += Dictionary.second.find(str[i])->second + MorseСode::sepSymb;
				break;
			}*/
			if (str[i] == wchar_t(" "[0]))
			{
				morseCode += MorseСode::sepWord;
				break;
			}
		}
	}
	//if (morseCode.empty()) throw L"Не найдено совпадений";

	return morseCode;
};

std::wstring MorseСode::MorseToChar(std::wstring str)
{
	std::wstring userStr;
	std::vector<std::wstring> words = Split(str, sepWord);
	std::vector<std::wstring> symbols;
	bool flag = false;

	for (std::wstring word : words)
	{
		symbols = Split(word, sepSymb);

		for (std::wstring symbol : symbols)
		{
			for (std::map<wchar_t, std::wstring>::const_iterator it = dictionaries.find(currentDictionary)->second.begin();
				it != dictionaries.find(currentDictionary)->second.end(); ++it)
			{
				if (it->second == symbol)
				{
					userStr += it->first;
					flag = true;
					break;
				}
			}
			// Обработка неопознанных символов по текущему словарю
			if (flag == false) userStr += L"#";
		}
		userStr += sepWord;
	}
	return userStr;
};

std::wstring MorseСode::SoundToMorse()
{

	return L"";
};

void MorseСode::PlayMorseCode(std::wstring& morseCode)
{
	std::vector<std::wstring> words = Split(morseCode, MorseСode::sepWord);
	std::vector<std::wstring> symbols;

	for (std::wstring word : words)
	{
		symbols = Split(word, MorseСode::sepSymb);

		for (std::wstring symbol : symbols)
		{
			for (size_t i = 0; i < symbol.length(); i++)
			{
				switch (symbol[i])
				{
				case '.':
					Beep(900, MorseСode::dotDuration);
					break;
				case '-':
					Beep(900, MorseСode::dashDuration);
					break;
				default:
					break;
				}
				Sleep(MorseСode::dotDuration);
			}
			Sleep(MorseСode::dashDuration);
		}
		Sleep(MorseСode::wordPauseDuration);
	}
	return;
};