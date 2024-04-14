#pragma once

#include <cstdint>
#include <string>
#include <windows.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <mmeapi.h>

struct RiffChunk {
    char chunkID[4];    // "RIFF"
    uint32_t chunkSize; // Размер всего файла - 8
    char format[4];     // "WAVE"
};

struct FmtSubChunk {
    char subchunk1Id[4];        // "fmt"
    uint32_t subchunk1Size;     // 16 для PCM
    WAVEFORMATEX waveFormatX;   // Формат аудиоданных
};

struct DataSubChunk {
    char subChunk2ID[4];     // "data"
    uint32_t subChunk2Size;  // Размер данных
};

struct WavHeader {
    RiffChunk riff = { { 'R', 'I', 'F', 'F' }, 0, { 'W', 'A', 'V', 'E' } };
    FmtSubChunk fmt = { { 'f', 'm', 't', ' ' }, 16 };
    DataSubChunk data = { { 'd', 'a', 't', 'a' }, 0 };
};