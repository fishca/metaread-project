#include "StringUtils.h"
#include <cwctype>
#include <string>
#include <cwchar>
#include <climits>
#include <cstdlib>
#include <algorithm>

namespace StringUtils {

    int ToIntDef(const std::wstring& s, int defaultValue) {
        if(s.empty()) return defaultValue;
        
        wchar_t* end;
        long value = std::wcstol(s.c_str(), &end, 10);
        
        if (end == s.c_str()) {
            // No conversion performed
            return defaultValue;
        }
        
        if (value == LONG_MAX || value == LONG_MIN) {
            if (errno == ERANGE) {
                // Out of range
                return defaultValue;
            }
        }
        
        return static_cast<int>(value);
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

    size_t LastDelimiter(const std::wstring& delimiters, const std::wstring& s) {
        if (s.empty()) return 0;
        
        size_t lastPos = 0;
        for (wchar_t delim : delimiters) {
            size_t pos = s.find_last_of(delim);
            if (pos != std::wstring::npos && pos > lastPos) {
                lastPos = pos;
            }
        }
        return lastPos == 0 ? 0 : lastPos + 1; // Возвращаем 1-based индекс как в оригинале
    }

}