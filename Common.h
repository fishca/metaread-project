//---------------------------------------------------------------------------

#ifndef CommonH
#define CommonH

#include <string>
#include <cstring>
#include <cwchar>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

// Определение базовых типов
typedef std::string AnsiString;
typedef std::wstring String;
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long ulong;

// Определение GUID структуры
struct TGUID {
	byte Data[16];
};

inline bool operator==(const TGUID& lhs, const TGUID& rhs) {
	return memcmp(lhs.Data, rhs.Data, 16) == 0;
}

inline bool operator!=(const TGUID& lhs, const TGUID& rhs) {
	return !(lhs == rhs);
}

// Use StringUtils for string operations
#include "StringUtils.h"

inline bool IsEmpty(const String& s) { return StringUtils::IsEmpty(s); }
inline int Compare(const String& s1, const String& s2) { return StringUtils::Compare(s1, s2); }
inline int CompareIC(const String& s1, const String& s2) { return StringUtils::CompareIC(s1, s2); }
inline size_t Pos(const String& substr, const String& s) { return StringUtils::Pos(s, substr); }
inline String SubString(const String& s, size_t start, size_t count) {
    return StringUtils::SubString(s, start - 1, count);
}
inline String UpperCase(const String& s) { return StringUtils::UpperCase(s); }
inline size_t Length(const String& s) { return s.size(); }

inline int ToIntDef(const String& s, int def) {
    return StringUtils::ToIntDef(s, def);
}
inline double ToDouble(const String& s) {
    return std::stod(s);
}
// Additional utility functions
String get_node_type_presentation(int node_type);
void CheckSynchronize(int timeout_ms = 0);
long InterlockedExchange(long volatile* target, long value);
long InterlockedIncrement(long volatile* target);
long InterlockedDecrement(long volatile* target);

// Определение базовых функций
bool string_to_GUID(const String& s, TGUID* g);
String GUID_to_string(const TGUID& g);

bool string1C_to_date(const String& s, char* d);
String date_to_string(const char* d);
String date_to_string1C(const char* d);

// Предварительное объявление TTempStream
class TTempStream;

// Обертки для функций CRC32 для совместимости
unsigned long _crc32_stream(TTempStream* stream);
unsigned long _crc32_string(const String& str);

inline unsigned long _crc32(TTempStream* stream) {
	return _crc32_stream(stream);
}

inline unsigned long _crc32(const String& str) {
	return _crc32_string(str);
}

// Определение базовых констант
extern TGUID EmptyUID;

// Определение enum типов, которые могут использоваться в других файлах
// Перемещены в NodeTypes.h для избежания дублирования

//---------------------------------------------------------------------------

#endif
