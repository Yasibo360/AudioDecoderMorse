#include "WavFileWriter.h"

WavFileWriter::WavFileWriter() {

}

WavFileWriter::~WavFileWriter() {

}

double WavFileWriter::GetSizeFile()
{
    double position = 0;
    double sizeFile = 0;

    // Запоминаем текущую позицию в файле
    position = outFile.tellp();

    if (outFile.is_open()) {
        outFile.seekp(0, std::ios::end);
        sizeFile = outFile.tellp();
    }

    // Возвращаем указатель на прежнюю позицию в файле
    outFile.seekp(position, std::ios::beg);
    return sizeFile;
}

void WavFileWriter::WriteHeader(WavHeader wavHeader) {
    if (outFile.is_open()) {
        double position = 0;
        position = outFile.tellp();

        // Запись заголовка WAV
        if (position != 0) {
            outFile.seekp(0, std::ios::beg);
        }

        int shift = 0;
        outFile.write(reinterpret_cast<const char*>(&wavHeader.riff), sizeof(wavHeader.riff));
        outFile.write(reinterpret_cast<const char*>(&wavHeader.fmt), sizeof(wavHeader.fmt));

        shift += sizeof(wavHeader.riff);
        shift += sizeof(wavHeader.fmt.subchunk1Id);
        shift += sizeof(wavHeader.fmt.subchunk1Size);
        shift += sizeof(wavHeader.fmt.waveFormatX);
        shift -= sizeof(wavHeader.fmt.waveFormatX.cbSize);

        outFile.seekp(shift, std::ios::beg);
        outFile.write(reinterpret_cast<const char*>(&wavHeader.data), sizeof(wavHeader.data));

        // Возвращаем указатель на прежнюю позицию в файле
        outFile.seekp(position, std::ios::beg);

    }
    else {
        throw std::runtime_error("Failed to write header to output file");
    }
}

void WavFileWriter::WriteData(std::ofstream outFile, const char* data, int length) {
    if (outFile.is_open()) {
        outFile.write(data, length);
    }
    else {
        throw std::runtime_error("Failed to write data to output file");
    }
}
