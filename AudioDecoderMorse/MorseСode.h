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

	/**
	 * Convert Morse code string to a character string.
	 *
	 * @param str the Morse code string to convert
	 *
	 * @return the character string decoded from Morse code
	 *
	 * @throws None
	 */
	std::vector<std::wstring> getAllDictionariesName();

	/**
	 * Retrieves the names of all available dictionaries.
	 *
	 * @return vector of wstrings containing the names of all dictionaries
	 *
	 * @throws None
	 */
	std::map<wchar_t, std::wstring> getAllDictionariesCombined(std::wstring alphabetName);

	/**
	 * A description of the entire C++ function.
	 *
	 * @param alphabetName description of parameter
	 *
	 * @return description of return value
	 *
	 * @throws ErrorType description of error
	 */
	std::wstring peakDurationsToMorse(const std::vector<std::pair<char, float>>& peakDurations, float precision = -2);

	std::vector<std::pair<float, float>> findWidePeaksInAudioFile(std::string filename);

	std::vector<std::pair<float, float>> findWidePeaks(std::vector<float>& samples, std::vector<float>& time, float period);

	/**
	 * A description of the entire C++ function.
	 *
	 * @param widePeaks description of parameter
	 * @param precision description of parameter
	 *
	 * @return description of return value
	 *
	 * @throws ErrorType description of error
	 */
	std::vector<std::pair<char, float>> findPeakDurations(std::vector<std::pair<float, float>>& widePeaks, float precision);

	/**
	 * Sets the threshold value for a particular operation.
	 *
	 * @param threshold the threshold value to be set
	 *
	 * @return void
	 *
	 * @throws None
	 */
	void setThreshold(float threshold);


	/**
	 * A description of the entire C++ function.
	 *
	 * @param N/A
	 *
	 * @return description of return value
	 *
	 * @throws N/A
	 */
	float getThreshold() const;

	/**
	 * A description of the entire C++ function.
	 *
	 * @param morseCode description of parameter
	 *
	 * @return description of return value
	 *
	 * @throws ErrorType description of error
	 */
	void playMorseCode(std::wstring& morseCode, int frequency = 1000);

};