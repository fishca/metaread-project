//---------------------------------------------------------------------------

#ifndef ConfigStorageH
#define ConfigStorageH

#include "Common.h"
#include "vcl.h"

// Forward declaration
struct ConfigFile;

//---------------------------------------------------------------------------
// Класс для хранения конфигурации
class ConfigStorage
{
private:
	String fpath;
public:
	ConfigStorage(const String& path) : fpath(path) {};
	~ConfigStorage() {};

	String presentation() const { return fpath; };
	bool fileexists(const String& filename) const;
	ConfigFile* readfile(const String& filename);
	void close(ConfigFile* cf);
};

//---------------------------------------------------------------------------
// Внешние файлы конфигурации
struct ConfigFile
{
	String name;
	TStream* str;
	ConfigFile(const String& _name, TStream* _str) : name(_name), str(_str) {};
	// Добавляем конструктор для TTempStream
	ConfigFile(const String& _name, void* _str) : name(_name), str(nullptr) {};
	~ConfigFile() {};
};

//---------------------------------------------------------------------------

#endif
