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

// Глобальные функции для String (VCL совместимость)
inline bool IsEmpty(const String& s) { return s.empty(); }
inline int Compare(const String& s1, const String& s2) { return s1.compare(s2); }
    String l = s1, r = s2;
    std::transform(l.begin(), l.end(), l.begin(), ::towlower);
    std::transform(r.begin(), r.end(), r.begin(), ::towlower);
    return l.compare(r);
}
inline String UpperCase(const String& s) {
    String result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::towupper);
    return result;
}
inline size_t Length(const String& s) { return s.size(); }

inline int CompareIC(const String& s1, const String& s2) {
    String s1_lower = s1;
    String s2_lower = s2;
    std::transform(s1_lower.begin(), s1_lower.end(), s1_lower.begin(), ::towlower);
    std::transform(s2_lower.begin(), s2_lower.end(), s2_lower.begin(), ::towlower);
    return s1_lower.compare(s2_lower);
}

inline size_t Pos(const String& substr, const String& s) {
    size_t pos = s.find(substr);
    return pos == String::npos ? 0 : pos + 1;
}

inline String SubString(const String& s, size_t start, size_t count) {
    if (start <= 0 || start > s.size()) return L"";
    return s.substr(start - 1, count);
}
inline size_t Pos(const String& s, const String& substr) {
    size_t pos = s.find(substr);
    return pos == String::npos ? 0 : pos + 1;
}
inline String SubString(const String& s, size_t start, size_t count) {
    if (start == 0) return String();
    size_t idx = start - 1;
    if (idx >= s.size()) return String();
    return s.substr(idx, count);
}
inline int ToIntDef(const String& s, int def) {
    try {
        return std::stoi(std::string(s.begin(), s.end()));
    } catch (...) {
        return def;
    }
}
inline double ToDouble(const String& s) {
    return std::stod(std::string(s.begin(), s.end()));
}

// String class extension methods (VCL compatibility) - removed to avoid duplication

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

// Определение базовых констант
extern TGUID EmptyUID;

// Определение enum типов, которые могут использоваться в других файлах
// Перемещены в NodeTypes.h для избежания дублирования

//---------------------------------------------------------------------------

#endif
