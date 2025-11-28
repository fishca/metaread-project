#include "StringUtils.h"
#include <cwctype>
#include <string>

namespace StringUtils {

int ToIntDef(const std::wstring& s, int defaultValue) {
    if(s.empty()) return defaultValue;
    try {
        return std::stoi(s);
    } catch (...) {
        return defaultValue;
    }
}

int Compare(const std::wstring& s1, const std::wstring& s2) {
    return s1.compare(s2);
}

int CompareIC(const std::wstring& s1, const std::wstring& s2) {
    std::wstring us1 = s1;
    std::wstring us2 = s2;
    std::transform(us1.begin(), us1.end(), us1.begin(), ::towupper);
    std::transform(us2.begin(), us2.end(), us2.begin(), ::towupper);
    return us1.compare(us2);
}

std::wstring SubString(const std::wstring& s, size_t start, size_t length) {
    if (start >= s.length()) return L"";
    if (start + length > s.length()) {
        return s.substr(start);
    }
    return s.substr(start, length);
}

size_t Pos(const std::wstring& s, const std::wstring& substr) {
    size_t pos = s.find(substr);
    return pos == std::wstring::npos ? 0 : pos + 1;
}

bool IsEmpty(const std::wstring& s) {
    return s.empty();
}

std::wstring UpperCase(const std::wstring& s) {
    std::wstring result = s;
    std::transform(result.begin(), result.end(), result.begin(), ::towupper);
    return result;
}
}