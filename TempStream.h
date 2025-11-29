//---------------------------------------------------------------------------

#ifndef TempStreamH
#define TempStreamH

#include <sstream>
#include <iostream>
#include "Common.h"
#include "vcl.h"

//---------------------------------------------------------------------------
// Класс для работы с потоками

class TTempStream
{
private:
	mutable std::stringstream buffer;
public:
	TTempStream() {};
	~TTempStream() {};

	void Write(const String& data);
	void Write(const char* data, int size);
	void Read(char* buffer_out, int size);
	void Seek(int pos, int origin);
	long Size() const;
	long Position() const;
	void CopyFrom(TTempStream* source, long size);
	
	// Дополнительные методы для работы с потоком
	void set_position(long pos) { Seek(pos, 0); }
	long get_position() const { return Position(); }
};


//---------------------------------------------------------------------------

#endif
