#pragma once
#include <map>
#include <string>

class Morse—ode
{
private:
	std::wstring dot;
	std::wstring dash;
	std::wstring sepSymb;
	std::wstring sepWord;

	std::map<wchar_t, std::wstring> activeDictionary;
	std::map<std::wstring, std::map<wchar_t, std::wstring>> dictionaries;

	std::wstring currentDictionary;

public:
	Morse—ode();
	~Morse—ode();

	int dotDuration;
	int dashDuration;
	int pauseDuration;
	int wordPauseDuration;

	void setCurrentDictionary(const std::wstring& dictionatyName);
	void setDurations(int dot, int dash, int wordPause, int letterPause);
	void loadDictionaryFromFile(const std::string& filename);
	std::wstring CharToMorse(std::wstring str);
	std::wstring MorseToChar(std::wstring str);
	std::wstring SoundToMorse();
	void PlayMorseCode(std::wstring& morseCode);
};