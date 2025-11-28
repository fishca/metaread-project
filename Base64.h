//---------------------------------------------------------------------------

#ifndef Base64H
#define Base64H

#include "Common.h"
#include "TempStream.h"
#include <string>
#include <vector>

//---------------------------------------------------------------------------
// Функции для работы с Base64

void base64_decode(const String& input, TTempStream* output, int prefix_len = 0);
void base64_encode(TTempStream* input, String& output, const String& prefix = L"");

bool string_to_GUID(const String& input, TGUID* output);
bool string_to_GUID(const String& input, TGUID* output);
String GUID_to_string(const TGUID& input);

//---------------------------------------------------------------------------

#endif
