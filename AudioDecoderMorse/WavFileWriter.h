#pragma once

#include "WavFileStructures.h"

class WavFileWriter
{
public:
    std::string fileName;
    std::ofstream outFile;
    WavHeader wavHeader;

    WavFileWriter();
    ~WavFileWriter();

    double GetSizeFile();

    void WriteHeader(WavHeader wavHeader);
    void WriteData(std::ofstream outFile, const char* data, int length);
};

