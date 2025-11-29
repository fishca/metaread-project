//---------------------------------------------------------------------------

#ifndef CRC32H
#define CRC32H

#include "Common.h"
#include "TempStream.h"
#include <string>

//---------------------------------------------------------------------------
// Функции для работы с CRC32

unsigned long _crc32_stream(TTempStream* stream);
unsigned long _crc32_string(const String& str);

//---------------------------------------------------------------------------

#endif
