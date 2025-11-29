#include "TempStream.h"
#include <string>

void TTempStream::Write(const String& data) {
    // Преобразуем wide string в UTF-8 для записи в поток
    std::string utf8_str;
    for (size_t i = 0; i < data.length(); i++) {
        wchar_t wc = data[i];
        if (wc < 0x80) {
            utf8_str += (char)wc;
        } else if (wc < 0x800) {
            utf8_str += (char)(0xC0 | (wc >> 6));
            utf8_str += (char)(0x80 | (wc & 0x3F));
        } else {
            utf8_str += (char)(0xE0 | (wc >> 12));
            utf8_str += (char)(0x80 | ((wc >> 6) & 0x3F));
            utf8_str += (char)(0x80 | (wc & 0x3F));
        }
    }
    buffer.write(utf8_str.c_str(), utf8_str.length());
}

void TTempStream::Write(const char* data, int size) {
    if (data && size > 0) {
        buffer.write(data, size);
    }
}

void TTempStream::Read(char* buffer_out, int size) {
    if (!buffer_out || size <= 0) {
        return;
    }
    
    buffer.read(buffer_out, size);
}

void TTempStream::Seek(int pos, int origin) {
    std::ios_base::seekdir dir;
    switch (origin) {
        case 0: dir = std::ios::beg; break;
        case 1: dir = std::ios::cur; break;
        case 2: dir = std::ios::end; break;
        default: dir = std::ios::beg; break;
    }
    buffer.seekg(pos, dir);
    buffer.seekp(pos, dir);
}

long TTempStream::Size() const {
    // Сохраняем текущее состояние
    std::streampos current_pos = buffer.tellg();
    buffer.seekg(0, std::ios::end);
    std::streampos size = buffer.tellg();
    buffer.seekg(current_pos);
    return size;
}

long TTempStream::Position() const {
    return buffer.tellg();
}

void TTempStream::CopyFrom(TTempStream* source, long size) {
    if (!source || size <= 0) {
        return;
    }
    
    long source_pos = source->Position();
    long source_size = source->Size();
    long bytes_to_copy = size;
    
    if (source_pos + bytes_to_copy > source_size) {
        bytes_to_copy = source_size - source_pos;
    }
    
    char* buffer_data = new char[bytes_to_copy];
    source->Read(buffer_data, bytes_to_copy);
    Write(buffer_data, bytes_to_copy);
    delete[] buffer_data;
}