#include "Base64.h"
#include "TempStream.h"
#include <vector>
#include <stdexcept>

static const std::wstring base64_chars = 
    L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    L"abcdefghijklmnopqrstuvwxyz"
    L"0123456789+/";

static inline bool is_base64(wchar_t c) {
    return (iswalnum(c) || (c == L'+') || (c == L'/'));
}

void base64_decode(const String& input, TTempStream* output, int prefix_len) {
    if (!output) return;
    
    int in_len = input.length();
    int i = 0;
    int in = 0;
    unsigned char char_array_4[4], char_array_3[3];
    
    // Пропускаем префикс
    in = prefix_len;
    
    while (in_len-- && (input[in] != L'=') && is_base64(input[in])) {
        char_array_4[i++] = input[in];
        in++;
        
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                output->Write((char*)&char_array_3[i], 1);
            
            i = 0;
        }
    }
    
    if (i) {
        for (int j = i; j < 4; j++)
            char_array_4[j] = 0;
        
        for (int j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (int j = 0; (j < i - 1); j++)
            output->Write((char*)&char_array_3[j], 1);
    }
}

void base64_encode(TTempStream* input, String& output, const String& prefix) {
    if (!input) return;
    
    output = prefix;
    std::vector<unsigned char> buffer;
    
    // Читаем все данные из потока
    long size = input->Size();
    long current_pos = input->Position();
    input->Seek(0, 0);
    
    buffer.resize(size);
    input->Read((char*)buffer.data(), size);
    
    // Восстанавливаем позицию потока
    input->Seek(current_pos, 0);
    
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (size--) {
        char_array_3[i++] = buffer[j++];
        
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for (i = 0; (i < 4); i++)
                output += base64_chars[char_array_4[i]];
            
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            output += base64_chars[char_array_4[j]];
        
        while ((i++ < 3))
            output += L'=';
    }
}