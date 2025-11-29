#include "CRC32.h"
#include "TempStream.h"

// Таблица CRC32 для быстрого вычисления
static unsigned long crc_table[256];
static bool crc_table_computed = false;

// Создаем таблицу CRC32
static void make_crc_table(void) {
    unsigned long c;
    int n, k;
    
    for (n = 0; n < 256; n++) {
        c = (unsigned long) n;
        for (k = 0; k < 8; k++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[n] = c;
    }
    crc_table_computed = true;
}

// Вычисление CRC32 для потока
unsigned long _crc32_stream(TTempStream* stream) {
    if (!stream) return 0;
    
    if (!crc_table_computed)
        make_crc_table();
    
    unsigned long crc = 0xffffffff;
    
    // Сохраняем текущую позицию потока
    long current_pos = stream->Position();
    
    // Переходим в начало потока
    stream->Seek(0, 0);
    
    // Читаем и обрабатываем данные
    char buffer[4096];
    long stream_size = stream->Size();
    long total_read = 0;
    
    while (total_read < stream_size) {
        int bytes_to_read = sizeof(buffer);
        if (total_read + bytes_to_read > stream_size) {
            bytes_to_read = stream_size - total_read;
        }
        
        stream->Read(buffer, bytes_to_read);
        unsigned char* buf = (unsigned char*) buffer;
        
        for (int i = 0; i < bytes_to_read; i++) {
            crc = crc_table[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
        }
        
        total_read += bytes_to_read;
    }
    
    // Восстанавливаем позицию потока
    stream->Seek(current_pos, 0);
    
    return crc ^ 0xffffffff;
}

// Вычисление CRC32 для строки
unsigned long _crc32_string(const String& str) {
    if (!crc_table_computed)
        make_crc_table();
    
    unsigned long crc = 0xffffffff;
    
    // Преобразуем wide string в байты для вычисления CRC
    for (size_t i = 0; i < str.length(); i++) {
        wchar_t wc = str[i];
        // Обрабатываем каждый байт wchar_t (обычно 2 байта)
        unsigned char c1 = wc & 0xFF;
        unsigned char c2 = (wc >> 8) & 0xFF;
        
        crc = crc_table[(crc ^ c1) & 0xff] ^ (crc >> 8);
        crc = crc_table[(crc ^ c2) & 0xff] ^ (crc >> 8);
    }
    
    return crc ^ 0xffffffff;
}