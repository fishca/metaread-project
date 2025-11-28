//---------------------------------------------------------------------------

#ifndef TempStreamH
#define TempStreamH

#include "Common.h"
#include <sstream>
#include <iostream>

//---------------------------------------------------------------------------
// Класс для работы с потоками

class TTempStream
{
private:
	std::stringstream buffer;
public:
	TTempStream() {};
	~TTempStream() {};

	void Write(const String& data) {};
	void Write(const char* data, int size) {};
	void Read(char* buffer, int size) {};
	void Seek(int pos, int origin) {};
	long Size() const { return 0; };
	long Position() const { return 0; };
	void CopyFrom(TTempStream* source, long size) {};
};


//---------------------------------------------------------------------------

#endif
