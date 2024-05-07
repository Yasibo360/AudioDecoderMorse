#include "Morse�ode.h"
#include "TextOperations.h"

#include <fstream>
#include <iostream>
#include <codecvt>
#include <cwctype>


Morse�ode::Morse�ode() : dot(L"."), dash(L"-"), sepSymb(L" "), sepWord(L"   "),
dotDuration(100), dashDuration(3 * dotDuration), pauseDuration(dotDuration),
wordPauseDuration(7 * dotDuration)
{
	// �������� ����������� ��������
	loadDictionaryFromFile("./Dictionaries/Dictionary Numeric.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary Service Symbols.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary En.txt");
	loadDictionaryFromFile("./Dictionaries/Dictionary Ru.txt");

	// ���� �������� �� �������, ��������� �� ������ ��������
	if (dictionaries.empty())
	{
		return;
	}
	else {
		// ������������� ������� ������� - ���������
		setCurrentDictionary(dictionaries.begin()->first);
	}
	return;
};

Morse�ode::~Morse�ode() {};

void Morse�ode::setCurrentDictionary(const std::wstring& dictionatyName)
{
	currentDictionary = dictionatyName;
	return;
};

void Morse�ode::setDurations(int dot, int dash, int letterPause, int wordPause)
{
	dotDuration = dot;
	dashDuration = dash;
	pauseDuration = letterPause;
	wordPauseDuration = wordPause;
};

void Morse�ode::loadDictionaryFromFile(const std::string& filename)
{
	std::wifstream file(filename); // �������� ����� ��� ������
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>)); // ��������� ������ ��� ���������� ��������� wchar_t

	if (!file.is_open()) {
		std::cerr << "�� ������� ������� ���� " << filename << std::endl;
		return;
	}

	std::wstring line;
	std::wstring alphabetName;

	// ������ �������� ��������
	if (std::getline(file, alphabetName)) {
		dictionaries[alphabetName]; // ���������� ������ ������ � �������� ����� ��� ������� �����

	}
	else {
		std::wcerr << L"���� ����." << std::endl;
		return;
	}

	// ������ ��������
	while (std::getline(file, line)) {
		if (line.empty()) return; // ���� ������ ������ = ������� ��������

		// ������� ������� ������� ������� ��� ����� ������
		size_t pos = line.find(L' ');
		if (pos == std::wstring::npos) {
			pos = line.size(); // ���� ������ �� ������, ���������� ����� ������
			return;
		}

		wchar_t character = line[0]; // ������ ������ - ��� ������ �����
		std::wstring code = line.substr(2, pos - 2); // ��������� ��� ����� �� ������� ������� ��� ����� ������

		// ���������� ���� ������-��� ����� � ��������������� ���������� �����
		dictionaries[alphabetName][character] = code;
	}
	file.close();

	return;
}

std::wstring Morse�ode::CharToMorse(std::wstring str)
{
	std::wstring morseCode{};

	// ��������� ������ ����� �� ���� ��������
	for (size_t i = 0; i < str.length(); i++)
	{
		str[i] = std::towupper(str[i]);
		for (const auto& dictionary : dictionaries) {
			if (dictionary.second.find(str[i]) != dictionary.second.end())
			{
				morseCode += dictionary.second.find(str[i])->second + Morse�ode::sepSymb;
				break;
			}

			/*if (Dictionary.second.begin()->first <= str[i] && str[i] <= Dictionary.second.rbegin()->first)
			{
				morseCode += Dictionary.second.find(str[i])->second + Morse�ode::sepSymb;
				break;
			}*/
			if (str[i] == wchar_t(" "[0]))
			{
				morseCode += Morse�ode::sepWord;
				break;
			}
		}
	}
	//if (morseCode.empty()) throw L"�� ������� ����������";

	return morseCode;
};

std::wstring Morse�ode::MorseToChar(std::wstring str)
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
			// ��������� ������������ �������� �� �������� �������
			if (flag == false) userStr += L"#";
		}
		userStr += sepWord;
	}
	return userStr;
};

std::wstring Morse�ode::SoundToMorse()
{

	return L"";
};

void Morse�ode::PlayMorseCode(std::wstring& morseCode)
{
	std::vector<std::wstring> words = Split(morseCode, Morse�ode::sepWord);
	std::vector<std::wstring> symbols;

	for (std::wstring word : words)
	{
		symbols = Split(word, Morse�ode::sepSymb);

		for (std::wstring symbol : symbols)
		{
			for (size_t i = 0; i < symbol.length(); i++)
			{
				switch (symbol[i])
				{
				case '.':
					Beep(900, Morse�ode::dotDuration);
					break;
				case '-':
					Beep(900, Morse�ode::dashDuration);
					break;
				default:
					break;
				}
				Sleep(Morse�ode::dotDuration);
			}
			Sleep(Morse�ode::dashDuration);
		}
		Sleep(Morse�ode::wordPauseDuration);
	}
	return;
};