#pragma once
#include <string>
#include <algorithm>

namespace StringUtils {
    int ToIntDef(const std::wstring& s, int defaultValue = 0);
    int Compare(const std::wstring& s1, const std::wstring& s2);
    int CompareIC(const std::wstring& s1, const std::wstring& s2);
    std::wstring SubString(const std::wstring& s, size_t start, size_t length);
    size_t Pos(const std::wstring& s, const std::wstring& substr);
    bool IsEmpty(const std::wstring& s);
    std::wstring UpperCase(const std::wstring& s);
    size_t LastDelimiter(const std::wstring& delimiters, const std::wstring& s);
}