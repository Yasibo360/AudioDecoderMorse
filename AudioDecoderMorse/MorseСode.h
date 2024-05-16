#pragma once

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <codecvt>
#include <cwctype>

class Morse—ode
{
private:
	std::wstring _dot;
	std::wstring _dash;
	std::wstring _sepSymb;
	std::wstring _sepWord;

	int _dotDuration;
	int _dashDuration;
	int _pauseDuration;
	int _wordPauseDuration;

	std::map<wchar_t, std::wstring> _activeDictionary;
	std::map<std::wstring, std::map<wchar_t, std::wstring>> _dictionaries;

	std::wstring _currentDictionary;

	float _threshold;

public:
	Morse—ode();

	~Morse—ode();	

	void setDurations(int dot, int dash, int wordPause, int letterPause);

	void setActiveDictionary(std::wstring);

	bool loadDictionaryFromFile(const std::string& filename);

	std::wstring charToMorse(std::wstring str);

	std::wstring morseToChar(std::wstring str);

	std::vector<std::wstring> getAllDictionariesName();

	std::map<wchar_t, std::wstring> getAllDictionariesCombined(std::wstring alphabetName);

	std::wstring peakDurationsToMorse(const std::vector<std::pair<char, float>>& peakDurations, float precision = -2);

	std::vector<std::pair<float, float>> findWidePeaksInAudioFile(std::string filename);

	std::vector<std::pair<float, float>> findWidePeaks(std::vector<float>& samples, std::vector<float>& time, float period);

	std::vector<std::pair<char, float>> findPeakDurations(std::vector<std::pair<float, float>>& widePeaks, float precision);

	void setThreshold(float threshold);
	float getThreshold() const;

	void playMorseCode(std::wstring& morseCode);

};