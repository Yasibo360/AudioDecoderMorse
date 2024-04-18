#pragma once

#include <process.h>
#include <cstdint>
#include <string>
#include "SFML/Graphics.hpp"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <sndfile.h>
#include <mmeapi.h>

class WavReaderWriter
{
private:
    SNDFILE* file;
    SF_INFO sfInfo;
    std::string fileName;
    bool fileOpenedForRead;
    bool fileOpenedForWrite;

public:
    WavReaderWriter();
    ~WavReaderWriter();
    
    void SetSF_Info(int samplerate, int channels, int format, int sections = 0, int seekable = 1);
    SF_INFO GetSF_Info() const;
    
    bool OpenFileForRead(const std::string& nameFile);
    bool OpenFileForWrite(const std::string& nameFile);

	void ReadData(short* data, size_t length);
    void WriteData(const short* data, size_t length);

    SNDFILE* GetFile();

    void SetFileName(const std::string& nameFile);
    std::string GetFileName();

    void CloseFile();
};