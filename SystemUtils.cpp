#include "Common.h"
#include "TempStream.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

// Реализация функции CreateDir
bool CreateDir(const String& dir) {
    // Преобразуем wide string в обычную строку
    std::string dir_name(dir.begin(), dir.end());
    
    // Создаем директорию с правами доступа 0755
    int result = mkdir(dir_name.c_str(), 0755);
    
    return (result == 0);
}

// Реализация функции DirectoryExists
bool DirectoryExists(const String& dir) {
    // Преобразуем wide string в обычную строку
    std::string dir_name(dir.begin(), dir.end());
    
    struct stat st;
    if (stat(dir_name.c_str(), &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    
    return false;
}

// Реализация функции get_file_format
int get_file_format(const String& filename) {
    // Преобразуем wide string в обычную строку
    std::string fname(filename.begin(), filename.end());
    
    // Открываем файл для чтения
    FILE* file = fopen(fname.c_str(), "rb");
    if (!file) {
        return -1; // Файл не найден
    }
    
    // Читаем первые байты для определения формата
    unsigned char header[8];
    size_t bytes_read = fread(header, 1, 8, file);
    fclose(file);
    
    if (bytes_read < 4) {
        return 0; // Неизвестный формат
    }
    
    // Проверяем на формат CF (1C)
    if (bytes_read >= 8 && 
        header[0] == 0xFF && header[1] == 0xFF && 
        header[2] == 0xFF && header[3] == 0xFF &&
        header[4] == 0x7F && header[5] == 0x54 &&
        header[6] == 0x43 && header[7] == 0x46) {
        return 1; // Формат CF
    }
    
    // Проверяем на формат EPF (1C)
    if (bytes_read >= 4 && 
        header[0] == '{' && header[1] == '"' &&
        header[2] == 'M' && header[3] == 'a') {
        return 2; // Формат EPF
    }
    
    return 0; // Неизвестный формат
}

// Реализация функции get_file_format для TTempStream
int get_file_format(TTempStream* stream) {
    if (!stream) {
        return -1; // Поток не найден
    }
    
    // Сохраняем текущую позицию
    long current_pos = stream->Position();
    
    // Переходим в начало потока
    stream->Seek(0, 0);
    
    // Читаем первые байты для определения формата
    unsigned char header[8];
    stream->Read((char*)header, 8);
    
    // Восстанавливаем позицию потока
    stream->Seek(current_pos, 0);
    
    // Проверяем на формат CF (1C)
    if (header[0] == 0xFF && header[1] == 0xFF && 
        header[2] == 0xFF && header[3] == 0xFF &&
        header[4] == 0x7F && header[5] == 0x54 &&
        header[6] == 0x43 && header[7] == 0x46) {
        return 1; // Формат CF
    }
    
    // Проверяем на формат EPF (1C)
    if (header[0] == '{' && header[1] == '"' &&
        header[2] == 'M' && header[3] == 'a') {
        return 2; // Формат EPF
    }
    
    return 0; // Неизвестный формат
}