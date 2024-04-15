#include "WavReaderWriter.h"

WavReaderWriter::WavReaderWriter() : file(nullptr), sfInfo{}, fileName(""), fileOpenedForRead(false), fileOpenedForWrite(false) {}

WavReaderWriter::~WavReaderWriter() {
    if (file != nullptr) {
        sf_close(file);
    }
}

void WavReaderWriter::SetSF_Info(int samplerate, int channels, int format, int sections, int seekable) {
    sfInfo.samplerate = samplerate;
    sfInfo.channels = channels;
    sfInfo.format = format;
    sfInfo.sections = sections;
    sfInfo.seekable = seekable;
}

SF_INFO WavReaderWriter::GetSF_Info() const {
    return sfInfo;
}

bool WavReaderWriter::OpenFileForRead(const std::string& nameFile) {
    if (fileOpenedForWrite) {
        fprintf(stderr, "Error: File is already open for writing\n");
        return false;
    }
    if (fileOpenedForRead) {
        fprintf(stderr, "Error: File is already open for reading\n");
        return false;
    }
    
    /*
    При открытии файла для чтения формат поле должно быть установлено на ноль перед вызовом sf_open().
    Единственным исключением из этого является случай файлов RAW, где вызывающий абонент должен установить 
    samplerate, channels и format поля допустимых значений. Все остальные поля структуры заполняются библиотекой.
    */
    switch (sfInfo.format) {
    case 0:
        sfInfo.format = 0;
        break;
    case SF_FORMAT_RAW:
        if (sfInfo.samplerate == 0) {
            sfInfo.samplerate = 44100;
        }
        if (sfInfo.channels == 0) {
            sfInfo.channels = 2;
        }
        break;
    default:
        break;
    }

    if (sf_format_check(&sfInfo)) {
        file = sf_open(nameFile.c_str(), SFM_READ, &sfInfo);
        if (file == nullptr) {
            fprintf(stderr, "Error opening file for reading\n");
            return false;
        }
    }
    
	fileName = nameFile;
    fileOpenedForRead = true;
    return true;
}

bool WavReaderWriter::OpenFileForWrite(const std::string& nameFile) {
    if (fileOpenedForRead) {
        fprintf(stderr, "Error: File is already open for reading\n");
        return false;
    }
    if (fileOpenedForWrite) {
        fprintf(stderr, "Error: File is already open for writing\n");
        return false;
    }

    // Проверяем существует ли файл
    std::ifstream fileCheck(nameFile); 
    if (!fileCheck.good()) {
        // Файл не существует, создаем его
        std::ofstream newFile(nameFile);
        if (!newFile.is_open()) {
            fprintf(stderr, "Error creating file\n");
            return false;
        }
    }

    // Проверяем корректность структуры SF_INFO
    if (sf_format_check(&sfInfo) == 1) {
        file = sf_open(nameFile.c_str(), SFM_WRITE, &sfInfo);
        if (file == nullptr) {
            fprintf(stderr, "Error opening file for writing\n");
            return false;
        }
    }
    else {
        fprintf(stderr, "Error checking file for writing\n");
        return false;
    }

    fileName = nameFile;
    fileOpenedForWrite = true;
    return true;
}

void WavReaderWriter::ReadData(short* data, size_t length) {
    if (!fileOpenedForRead) {
        fprintf(stderr, "Error: File is not open for reading\n");
        return;
    }

    if (sf_readf_short(file, data, length) != static_cast<sf_count_t>(length)) {
        fprintf(stderr, "Error: Failed to read data from file\n");
    }
}

void WavReaderWriter::WriteData(const short* data, size_t length) {
    if (!fileOpenedForWrite) {
        fprintf(stderr, "Error: File is not open for writing\n");
        return;
    }
    //sf_write_raw(file, data, length); // Записать аудиоданные в файл
    if (sf_writef_short(file, data, length) != static_cast<sf_count_t>(length)) {
       fprintf(stderr, "Error: Failed to write data to file\n");
    }
}

void WavReaderWriter::SetFileName(const std::string& nameFile) {
    fileName = nameFile;
}

std::string WavReaderWriter::GetFileName() {
    return fileName;
}

void WavReaderWriter::CloseFile() {
    if (file != nullptr) {
        sf_close(file);

        file = nullptr;
        fileOpenedForRead = false;
        fileOpenedForWrite = false;
    }
}