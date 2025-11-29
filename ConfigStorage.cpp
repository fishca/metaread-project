#include "ConfigStorage.h"
#include "TempStream.h"
#include <fstream>
#include <sys/stat.h>

bool ConfigStorage::fileexists(const String& filename) const {
    // Преобразуем wide string в обычную строку для проверки файла
    std::string fname(filename.begin(), filename.end());
    struct stat buffer;
    return (stat(fname.c_str(), &buffer) == 0);
}

ConfigFile* ConfigStorage::readfile(const String& filename) {
    if (!fileexists(filename)) {
        return nullptr;
    }
    
    // Преобразуем wide string в обычную строку для открытия файла
    std::string fname(filename.begin(), filename.end());
    std::ifstream file(fname.c_str(), std::ios::binary | std::ios::ate);
    
    if (!file.is_open()) {
        return nullptr;
    }
    
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Создаем временный поток для хранения данных
    TTempStream* stream = new TTempStream();
    
    if (size > 0) {
        std::vector<char> buffer(size);
        if (!file.read(buffer.data(), size)) {
            delete stream;
            return nullptr;
        }
        
        // Записываем данные в поток
        stream->Write(buffer.data(), size);
    }
    
    file.close();
    
    return new ConfigFile(filename, stream);
}

void ConfigStorage::close(ConfigFile* cf) {
    if (cf) {
        if (cf->str) {
            delete cf->str;
        }
        delete cf;
    }
}