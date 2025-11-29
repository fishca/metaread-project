//---------------------------------------------------------------------------
// Заголовочный файл для совместимости с VCL (Visual Component Library)

#ifndef vclH
#define vclH

#include "Common.h"
// Windows.h не требуется для GCC компиляции
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <cstdint>


// Определение базовых типов и констант VCL для совместимости
typedef std::string AnsiString;
typedef std::wstring WideString;
typedef std::wstring String;

// Определение потоков
class TThread
{
public:
	TThread(bool suspended) {};
	virtual ~TThread() {};
	virtual void Start() {};
	virtual void WaitFor() {};
	void Terminate() {};
	void Suspend() {};
	void Resume() {};
};

class TEvent
{
public:
	TEvent(void* event, bool manual_reset, bool initial_state, const String& name, bool open_existing = false) {};
	~TEvent() {};
	void SetEvent() {};
	void ResetEvent() {};
	int WaitFor(int) { return 0; };
};

// Определение потоков
class THandle
{
public:
	THandle() : handle(0) {};
	~THandle() {};
	void* handle;
};

// Определение потоков
class TStream
{
public:
	TStream() {};
	virtual ~TStream() {};
	virtual void Write(const char* buffer, int count) {};
	virtual void Read(char* buffer, int count) {};
	virtual long Size() const { return 0; };
	virtual long Position() const { return 0; };
	virtual void Seek(long offset, int origin) {};
	virtual void CopyFrom(TStream* source, long count) {};
};

// Определение стримеров
class TStreamWriter
{
private:
	std::ofstream* file;
	TStream* stream;
public:
	TStreamWriter(std::ofstream* f) : file(f), stream(nullptr) {};
	TStreamWriter(TStream* str, void* encoding, int buffer_size) : file(nullptr), stream(str) {};
	// Добавляем конструктор для TTempStream
	TStreamWriter(void* temp_stream, void* encoding, int buffer_size) : file(nullptr), stream(nullptr) {};
	~TStreamWriter() {};
	void Write(const String& text) {};
	void WriteLine(const String& text) {};
};

class TStreamReader
{
private:
	std::ifstream* file;
	TStream* stream;
public:
	TStreamReader(std::ifstream* f) : file(f), stream(nullptr) {};
	TStreamReader(TStream* str, bool detect_encoding) : file(nullptr), stream(str) {};
	~TStreamReader() {};
	String ReadLine() { return L""; };
	String ReadToEnd() { return L""; };
	int Read() { return -1; };
};

class TMemoryStream : public TStream
{
public:
	TMemoryStream() {};
	~TMemoryStream() {};
};

class TStringStream : public TStream
{
private:
	String str;
public:
	TStringStream(const String& s) : str(s) {};
	~TStringStream() {};
	String GetDataString() const { return str; };
};

// Определение файловых потоков
class TFileStream : public TStream
{
private:
	String fname;
	int mode;
public:
	TFileStream(const String& filename, int filemode) : fname(filename), mode(filemode) {};
	~TFileStream() {};
	String getFileName() const { return fname; };
};

// Определение перечисления режимов файла
enum TFileMode {
	fmCreate = 1,
	fmOpenRead = 2,
	fmOpenWrite = 3,
	fmOpenAppend = 4
};

// Определение функций для работы с файлами
bool DirectoryExists(const String& path);
bool CreateDir(const String& path);
bool FileExists(const String& filename);
void DeleteFile(const String& filename);

// Определение функций для работы с потоками
TStream* OpenFileStream(const String& filename, int mode);

// Определение функций для работы с текстом
class TStringBuilder
{
private:
	String str;
public:
	TStringBuilder() : str(L"") {};
	TStringBuilder(const String& s) : str(s) {};
	~TStringBuilder() {};
	void Replace(const String& old_str, const String& new_str) {};
	void Clear() { str = L""; };
	void Append(wchar_t c) { str += c; };
	void Append(const String& s) { str += s; };
	String ToString() const { return str; };
};

// Определение функций для работы с кодировками
class TEncoding
{
public:
	static String UTF8;
};

inline std::vector<uint8_t> GetPreamble()
{
	std::vector<uint8_t> preamble(3);
	preamble[0] = 0xEF;
	preamble[1] = 0xBB;
	preamble[2] = 0xBF;
	return preamble;
}

// Определение функций для работы с регистратором сообщений
class MessageRegistrator
{
public:
	void AddError(const std::wstring& msg) {};
	void AddError(const std::wstring& msg, const std::wstring& param1_name, const std::wstring& param1_value) {};
	void AddError(const std::wstring& msg, const std::wstring& param1_name, const std::wstring& param1_value, const std::wstring& param2_name, const std::wstring& param2_value) {};
	void AddError(const std::wstring& msg, const std::wstring& param1_name, const std::wstring& param1_value, const std::wstring& param2_name, const std::wstring& param2_value, const std::wstring& param3_name, const std::wstring& param3_value) {};
};

// Определение глобальных переменных
extern MessageRegistrator* msreg;

//---------------------------------------------------------------------------

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

// Определение перечисления для замены строк
enum TReplaceFlags {
	rfReplaceAll = 1
};


// Функция для замены строк
inline String StringReplace(const String& source, const String& oldStr, const String& newStr, TReplaceFlags flags) {
	String result = source;
	if (flags & rfReplaceAll) {
		size_t pos = 0;
		while ((pos = result.find(oldStr, pos)) != String::npos) {
			result.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}
	}
	return result;
}

// Функция для преобразования в hex
inline String tohex(int value) {
	std::wstringstream ss;
	ss << std::hex << value;
	return ss.str();
}

#endif
