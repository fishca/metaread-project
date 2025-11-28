//---------------------------------------------------------------------------

#ifndef MetaDataH
#define MetaDataH

#include <vector>
#include <map>

#include "NodeTypes.h"
#include "Parse_tree.h"
#include "Common.h"
#include "ConfigStorage.h"
#include "TempStream.h"

//---------------------------------------------------------------------------
extern TGUID EmptyUID;

//---------------------------------------------------------------------------
class MetaBase;
class MetaValue;
class MetaProperty;
struct MetaPropertyLess;
struct GeneratedType;
class Class;
class MetaType;
class MetaTypeSet;
class Value1C;
class Value1C_obj;
class Value1C_metaobj;
class MetaContainer;
class ConfigStorage;
struct VarValidValue;
class Value1C_obj_ExportThread;


//---------------------------------------------------------------------------
// Базовый класс метаданных 1С
class MetaBase
{
protected:
	String fname;
	String fename;
public:
	MetaBase(){};
	MetaBase(
		const String& _name
		,const String& _ename
	) : fname(_name), fename(_ename){};
	~MetaBase(){};
	void setname(const String& _name){fname = _name;};
	void setename(const String& _ename){fename = _ename;};
	String getname() const { return fname; };
	String getename() const { return fename; };
	String getname(bool english = false)
	{
		if(english) return fename;
		else return fname;
	};
};

//---------------------------------------------------------------------------
// Предопределенное значение метаданных
class MetaValue : public MetaBase
{
protected:
	MetaType* owner;
	int fvalue;
	TGUID fvalueUID;
public:
	MetaValue(
		MetaType* _owner,
		const String& _name,
		const String& _ename,
		const int _value
	) : MetaBase(_name, _ename), fvalue(_value), owner(_owner){};
	MetaValue(MetaType* _owner, tree* tr);
	~MetaValue(){};
	int getvalue() const { return fvalue; };
	TGUID getvalueUID() const { return fvalueUID; };
	MetaType* getowner(){return owner;};
};


//---------------------------------------------------------------------------
// Свойство метаданных
class MetaProperty : public MetaBase
{
protected:
	std::vector<MetaType*> ftypes;
	std::vector<String> fstypes;
	MetaType* owner;
	bool fpredefined;
	ExportType fexporttype;
	Class* f_class;
public:
	DefaultValueType defaultvaluetype;
	union
	{
		bool dv_bool;
		int dv_number;
		String* dv_string;
		char dv_date[7];
		MetaType* dv_type;
		MetaValue* dv_enum;
	};

	MetaProperty(
		MetaType* _owner,
		const String& _name,
		const String& _ename
	) : MetaBase(_name, _ename), owner(_owner){};
	MetaProperty(MetaType* _owner, tree* tr);
	~MetaProperty();
	void filltypes();
	std::vector<MetaType*> gettypes() const { return ftypes; };
	MetaType* getowner(){return owner;};
	bool getpredefined() const { return fpredefined; };
	ExportType getexporttype() const { return fexporttype; };
	Class* get_class() const { return f_class; };

};

//---------------------------------------------------------------------------
// Сравнение свойств метаданных
struct MetaPropertyLess
{
	bool operator()(MetaProperty* const l, MetaProperty* const r) const
	{
		return l->getname() < r->getname();
	}
};

//---------------------------------------------------------------------------
// Объект метаданных
class MetaObject : public MetaBase
{
protected:
	String ffullname;
	String fefullname;
	TGUID fuid;
	Value1C_metaobj* fvalue;
public:
	static std::map<TGUID, MetaObject*> map;
	static std::map<String, MetaObject*> smap;

	MetaObject(const TGUID& _uid, Value1C_metaobj* _value) : fuid(_uid), fvalue(_value){};
	MetaObject(const TGUID& _uid, Value1C_metaobj* _value, const String _name, const String _ename) : MetaBase(_name, _ename), fuid(_uid), fvalue(_value){};
	void setfullname(const String& _fullname){ffullname = _fullname;};
	void setefullname(const String& _efullname){fefullname = _efullname;};
	String getfullname() const { return ffullname; };
	String getefullname() const { return fefullname; };
	TGUID getuid() const { return fuid; };
	Value1C_metaobj* getvalue() const { return fvalue; };
	String getfullname(bool english = false)
	{
		if(english) return fefullname;
		else return ffullname;
	};
};

//---------------------------------------------------------------------------
// Генерируемый тип
class MetaGeneratedType : public MetaBase
{
protected:
	bool fwoprefix; // Признак "Без префикса"
public:
	bool getwoprefix() const { return fwoprefix; };

	MetaGeneratedType(
		const String& _name,
		const String& _ename
	) : MetaBase(_name, _ename), fwoprefix(false)
	{};
	MetaGeneratedType(tree* tr);

};

//---------------------------------------------------------------------------
// Право
class MetaRight : public MetaBase
{
protected:
	TGUID fuid;
	Version1C fver1C;
public:
	static std::map<TGUID, MetaRight*> map;
	static std::map<String, MetaRight*> smap;

	//MetaRight(const TGUID& _uid) : fuid(_uid){map[fuid] = this;};
	//MetaRight(const TGUID& _uid, const String _name, const String _ename) : MetaBase(_name, _ename), fuid(_uid){
	//	map[fuid] = this;
	//	smap[fname] = this;
	//	smap[fename] = this;
	//};
	MetaRight(tree* tr);
	static MetaRight* getright(const TGUID& _uid);
	static MetaRight* getright(const String& _name);
	TGUID getuid() const { return fuid; };
	Version1C getver1C() const { return fver1C; };
};



//---------------------------------------------------------------------------
// Стандартный реквизит
class MetaStandartAttribute : public MetaBase
{
protected:
	int fvalue;
	bool fcount;
	int fvaluemax;
	TGUID fuid;
public:
	MetaStandartAttribute(
		const String& _name,
		const String& _ename
	) : MetaBase(_name, _ename)
	{fcount = false;};
	MetaStandartAttribute(tree* tr);
	int getvalue() const { return fvalue; };
	bool getcount() const { return fcount; };
	int getvaluemax() const { return fvaluemax; };
	TGUID getuid() const { return fuid; };
};

//---------------------------------------------------------------------------
// Стандартная табличная часть
class MetaStandartTabularSection : public MetaBase
{
protected:
	int fvalue;
	Class* f_class;
public:
	TGUID class_uid;
	static std::vector<MetaStandartTabularSection*> list;

	MetaStandartTabularSection(
		const String& _name,
		const String& _ename
	) : MetaBase(_name, _ename)
	{
		f_class = NULL;
		class_uid = EmptyUID;
	};
	MetaStandartTabularSection(tree* tr);
	int getvalue() const { return fvalue; };
	Class* get_class() const { return f_class; };
	void set_class(Class* cl) { f_class = cl; };
};

//---------------------------------------------------------------------------
// Параметры классов
class ClassParameter
{
private:
	String fname;
	static std::map<String, ClassParameter*> map;
public:
	//ClassParameter(String _name) : fname(_name){map[fname] = this;};
	ClassParameter(tree* tr);
	String getname() const { return fname; };
	static ClassParameter* getparam(const String& paramname);
};

//---------------------------------------------------------------------------
// Классы
class Class
{
private:
	TGUID fuid;
	std::vector<VarValidValue> fvervalidvalues;
	std::map<ClassParameter*, int> fparamvalues;
	static std::map<TGUID, Class*> map;
	std::vector<MetaStandartAttribute*> fstandartattributes; // Стандартные реквизиты
	std::vector<MetaStandartTabularSection*> fstandarttabularsections; // Стандартные табличные части

public:
	Class(tree* tr);
	~Class();

	TGUID getuid() const { return fuid; };
	std::vector<VarValidValue> getvervalidvalues() const { return fvervalidvalues; };
	std::map<ClassParameter*, int> getparamvalues() const { return fparamvalues; };
	std::vector<MetaStandartAttribute*> getstandartattributes() const { return fstandartattributes; };
	std::vector<MetaStandartTabularSection*> getstandarttabularsections() const { return fstandarttabularsections; };

	int getparamvalue(ClassParameter* p);
	static Class* getclass(const TGUID& id);

};

//---------------------------------------------------------------------------
// Экземпляр класса
class ClassItem
{
private:
	Class* fcl;
	bool fversionisset;
	int fversion;
public:
	ClassItem(Class* _cl) : fcl(_cl){fversionisset = false;};
	Class* getcl() const { return fcl; };
	int getversion() const { return fversion; };
	void setversion(int v) { fversion = v; };
};

//---------------------------------------------------------------------------
// Допустимое значение переменной дерева сериализации
struct VarValidValue
{
	int value;
	Version1C ver1C;
	int globalvalue;
};

//---------------------------------------------------------------------------
// Переменная дерева сериализации
class SerializationTreeVar
{
private:
	String fname;
	bool fcolcount;
	bool fisglobal;
	bool fisfix;
	int ffixvalue;
	std::vector<VarValidValue> fvalidvalues;
public:
	SerializationTreeVar(tree* tr);
	String getname() const { return fname; };
	bool getcolcount() const { return fcolcount; };
	bool getisglobal() const { return fisglobal; };
	bool getisfix() const { return fisfix; };
	int getfixvalue() const { return ffixvalue; };
	std::vector<VarValidValue> getvalidvalues() const { return fvalidvalues; };
};








//---------------------------------------------------------------------------
// Узел дерева сериализации
struct SerializationTreeNode
{
	SerializationTreeNode* next; // следующий на этом уровне
	SerializationTreeNode* first; // первый на подчиненном уровне
	SerializationTreeNode* parent; // родитель
	unsigned int index; // индекс на текущем уровне
	MetaType* owner;

	SerializationTreeNodeType type;
	SerializationTreeCondition condition; // (type == stt_cond)

	SerializationTreeValueType typeval1; // Тип значения 1 (type == stt_const ИЛИ type == stt_cond ИЛИ type == stt_elcol)
	String str1; // ((type == stt_const ИЛИ type == stt_cond ИЛИ type == stt_var ИЛИ type == stt_elcol) И typeval1 = stv_string ИЛИ typeval1 = stv_var ИЛИ typeval1 = stv_globalvar)
	union
	{
		int num1; // ((type == stt_const ИЛИ type == stt_cond ИЛИ type == stt_elcol) И typeval1 = stv_number)
		TGUID uid1; // (((type == stt_const ИЛИ type == stt_cond) И typeval1 = stv_uid) ИЛИ type == stt_class ИЛИ type == stt_idel)
		MetaValue* val1; // значение (type == stt_cond И typeval1 = stv_value)
		MetaProperty* prop1; // свойство (type == stt_prop ИЛИ ((type == stt_cond ИЛИ type == stt_elcol) И typeval1 = stv_prop))
		MetaGeneratedType* gentype; // генерируемый тип (type == stt_gentype)
		ContainerVer vercon1; // версия контейнера (type == stt_cond И typeval1 = stv_vercon)
		Version1C ver1C1; // версия 1С (type == stt_cond И typeval1 = stv_ver1С)
		SerializationTreeClassType classtype; // вид коллекции классов ((type == stt_classcol)
		ClassParameter* classpar1; // параметр класса (type == stt_cond И typeval1 = stv_classpar)
	};
	MetaType* typeprop; // тип свойства (type == stt_prop ИЛИ type == stt_elcol ИЛИ type == stt_idel)

	SerializationTreeValueType typeval2; // Тип значения 2 (type == stt_cond)
	String str2; // (type == stt_cond И (typeval2 = stv_string ИЛИ typeval2 = stv_var ИЛИ typeval2 = stv_globalvar))
	union
	{
		int num2; // (type == stt_cond И typeval2 = stv_number)
		TGUID uid2; // (type == stt_cond И typeval2 = stv_uid)
		MetaValue* val2; // значение (type == stt_cond И typeval2 = stv_value)
		MetaProperty* prop2; // свойство (type == stt_prop ИЛИ (type == stt_cond И typeval2 = stv_prop))
		ContainerVer vercon2; // версия контейнера (type == stt_cond И typeval2 = stv_vercon)
		Version1C ver1C2; // версия 1С ((type == stt_cond И typeval2 = stv_ver1С) ИЛИ type == stt_class ИЛИ type == stt_idel)
		ClassParameter* classpar2; // параметр класса (type == stt_cond И typeval2 = stv_classpar)
	};

	bool nomove; //
	bool isref; // Признак ссылки на объект метаданных (т.е. тут есть только UID, самого определения объекта метаданных тут нет)
	bool isrefpre; // Признак ссылки на предопределенный элемент (т.е. тут есть только UID, самого предопределенного элемента тут нет)
	bool isright; // Это право
	bool exnernal; // Объект находится во внешнем файле, тут только UID, который является именем внешнего файла
	BinarySerializationType binsertype;
	ExternalFileFormat binformat;

	SerializationTreeNode()
	{
		next = NULL;
		first = NULL;
		nomove = false;
	};
	SerializationTreeNode(MetaType* _owner, tree* tr, SerializationTreeNode* _parent, unsigned int _index);
	~SerializationTreeNode();
	String path() const;
	static SerializationTreeNode* SerializationTree(MetaType* _owner, tree* tr, SerializationTreeNode* parent);
	static String typevalpresentation(SerializationTreeValueType typeval);
	String typeval1presentation() const{return typevalpresentation(typeval1);};
	String typeval2presentation() const{return typevalpresentation(typeval2);};
};

//---------------------------------------------------------------------------
// Внешний файл типа
struct ExternalFile
{
	MetaType* owner;
	MetaProperty* prop; // Свойство
	bool relativepath; // признак относительного пути файла
	String name; // Имя
	String ext; // Расширение (без точки впереди)
	bool catalog; // признак каталога (если catalog == true, то это файл <name+"."+ext+"/"+filename>, иначе <name+"."+ext>)
	String filename; // Имя файла (только если catalog == true)
	ExternalFileFormat format; // Формат файла
	MetaType* type; // Тип свойства
	bool optional; // признак необязательности
	Version1C ver1C; // Минимальная версия 1С
	bool havecondition; // признак наличия условия
	SerializationTreeCondition condition; // (type == stt_cond)

	SerializationTreeValueType typeval1; // Тип значения 1 (havecondition == true)
	String str1; // typeval1 = stv_string ИЛИ typeval1 = stv_var ИЛИ typeval1 = stv_globalvar
	union
	{
		int num1; // число (typeval1 = stv_number)
		TGUID uid1; // УИД (typeval1 = stv_uid)
		MetaValue* val1; // значение (typeval1 = stv_value)
		MetaProperty* prop1; // свойство (typeval1 = stv_prop)
		ContainerVer vercon1; // версия контейнера (typeval1 = stv_vercon)
		Version1C ver1C1; // версия 1С (typeval1 = stv_ver1С)
		ClassParameter* classpar1; // параметр класса (typeval1 = stv_classpar)
	};

	SerializationTreeValueType typeval2; // Тип значения 2 (havecondition == true)
	String str2; // typeval2 = stv_string ИЛИ typeval2 = stv_var ИЛИ typeval2 = stv_globalvar
	union
	{
		int num2; // число (typeval2 = stv_number)
		TGUID uid2; // УИД (typeval2 = stv_uid)
		MetaValue* val2; // значение (typeval2 = stv_value)
		MetaProperty* prop2; // свойство (typeval2 = stv_prop)
		ContainerVer vercon2; // версия контейнера (typeval2 = stv_vercon)
		Version1C ver1C2; // версия 1С (typeval2 = stv_ver1С)
		ClassParameter* classpar2; // параметр класса (typeval2 = stv_classpar)
	};

	ExternalFile(MetaType* _owner, tree* tr);
	String typeval1presentation() const{return SerializationTreeNode::typevalpresentation(typeval1);};
	String typeval2presentation() const{return SerializationTreeNode::typevalpresentation(typeval2);};
};

//---------------------------------------------------------------------------
// Тип метаданных
class MetaType : public MetaBase
{
private:
	void init();
protected:
	MetaTypeSet* typeSet; // набор типов, которому принадлежит этот тип

	TGUID fuid; // УИД типа
	bool fhasuid; // Признак наличия УИД
	String fmetaname;
	String femetaname;
	String fgentypeprefix;
	String fegentypeprefix;
	unsigned int fserialization_ver; // Вариант сериализации
	int fimageindex; // индекс картинки
	unsigned int fprenameindex; // Индекс колонки имя предопределенного
	unsigned int fpreidindex; // ИндексКолонкиИДПредопределенного
	std::vector<MetaProperty*> fproperties; // Свойства
	std::map<String, MetaProperty*> fproperties_by_name; // Соответствие имен (русских и английских) свойствам
	std::vector<MetaValue*> fvalues; // Предопределенные значения типа
	std::map<int, MetaValue*> fvalues_by_value; // Соответствие числовых значений предопределенным значениям
	std::map<String, MetaValue*> fvalues_by_name; // Соответствие имен (русских и английских) предопределенным значениям
	std::vector<MetaType*> fcollectiontypes; // Типы элементов коллекции
	std::vector<String> fscollectiontypes; // Имена типов элементов коллекции
	std::vector<MetaGeneratedType*> fgeneratedtypes; // Генерируемые типы
	bool fmeta; // Признак объекта метаданных
	ExportType fexporttype;
	Class* fdefaultclass;

	// Дерево сериализации
	std::vector<SerializationTreeVar*> fserializationvars;
	SerializationTreeNode* fserializationtree; // Если NULL - дерева сериализации нет
	std::vector<ExternalFile*> fexternalfiles;

public:
	MetaType(
		MetaTypeSet* _typeSet,
		const String& _name,
		const String& _ename,
		const String& _metaname,
		const String& _emetaname,
		const String& _uid
	);
	MetaType(
		MetaTypeSet* _typeSet,
		const String& _name,
		const String& _ename,
		const String& _metaname,
		const String& _emetaname,
		const TGUID& _uid
	);
	MetaGeneratedType* gentypeRef; // генерируемый тип Ссылка
	MetaType(MetaTypeSet* _typeSet, tree* tr);
	~MetaType();
	void setSerializationTree(tree* tr);
	String getmetaname() const { return fmetaname; };
	String getemetaname() const { return femetaname; };
	String getgentypeprefix() const { return fgentypeprefix; };
	String getegentypeprefix() const { return fegentypeprefix; };
	bool gethasuid() const { return fhasuid; };
	TGUID getuid() const { return fuid; };
	unsigned int getserialization_ver() const { return fserialization_ver; };
	int getimageindex() const { return fimageindex; };
	unsigned int getprenameindex() const { return fprenameindex; };
	unsigned int getpreidindex() const { return fpreidindex; };
	std::vector<MetaProperty*> getproperties() const { return fproperties; };
	std::vector<MetaValue*> getvalues() const { return fvalues; };
	std::vector<MetaType*> getcollectiontypes() const { return fcollectiontypes; };
	std::vector<MetaGeneratedType*> getgeneratedtypes() const { return fgeneratedtypes; };
	MetaTypeSet* getTypeSet() const { return typeSet; };
	std::vector<SerializationTreeVar*> getserializationvars() const { return fserializationvars; };
	SerializationTreeNode* getserializationtree() const { return fserializationtree; };
	std::vector<ExternalFile*> getexternalfiles() const { return fexternalfiles; };
	bool getmeta() const { return fmeta; };
	ExportType getexporttype() const { return fexporttype; };
	Class* getdefaultclass() const { return fdefaultclass; };

	MetaProperty* getProperty(const String& n);
	MetaProperty* getProperty(int index);
	MetaValue* getValue(const String& n);
	MetaValue* getValue(int value);
	int numberOfProperties();
	void fillcollectiontypes(); // Заполнить типы элементов коллекции по их именам (по fscollectiontypes заполнить fcollectiontypes)
	String getmetaname(bool english = false)
	{
		if(english) return femetaname;
		else return fmetaname;
	}

};

//---------------------------------------------------------------------------
// Набор типов метаданных (статических или генерируемых)
class MetaTypeSet
{
private:
	std::map<String, MetaType*> mapname; // соответствие имен (русских и английских) типам
	std::map<TGUID, MetaType*> mapuid; // соответствие идентификаторов типам
	std::vector<MetaType*> alltype; // массив всех типов

public:
	static MetaTypeSet* staticTypes; // Cтатические типы
	// Пустой тип
	static MetaType* mt_empty;
	// Примитивные типы
	static MetaType* mt_string;
	static MetaType* mt_number;
	static MetaType* mt_bool;
	static MetaType* mt_date;
	static MetaType* mt_undef;
	static MetaType* mt_null;
	static MetaType* mt_type;
	// УникальныйИдентификатор
	static MetaType* mt_uid;
	// ОписаниеТипаВнутр
	static MetaType* mt_typedescrinternal;
	// Двоичные данные
	static MetaType* mt_binarydata;
	// Произвольный тип
	static MetaType* mt_arbitrary;
	// Корневой тип
	static MetaType* mt_container;
	static MetaType* mt_config;
	// Псевдо-тип Стандартный атрибут
	static MetaType* mt_standart_attribute;
	// Псевдо-тип Стандартная табличная часть
	static MetaType* mt_standart_tabular_section;
	// Значения частей даты для квалификатора даты
	static MetaValue* mv_datefractionsdate;
	static MetaValue* mv_datefractionstime;
	// Тип ЧастиДаты
	static MetaType* mt_datefractions;
	// Свойство ЧастиДаты типа КвалификаторыДаты
	static MetaProperty* mp_datefractions;
	// ОбъектМетаданных: ТабличнаяЧасть
	static MetaType* mt_tabularsection;
	// ОбъектМетаданных: Реквизит
	static MetaType* mt_attribute;
	// ОбъектМетаданныхСсылка
	static MetaType* mt_metaobjref;
	// ОбъектМетаданныхСсылкаВнутр
	static MetaType* mt_metarefint; // специальный тип для свойств с галочкой Ссылка в деревьях сериализации
	// ТабличнаяЧасть
	static MetaType* mt_tabsection;
	// МетаСсылка
	static MetaType* mt_metaref;

	MetaTypeSet(){};
	~MetaTypeSet();
	MetaType* getTypeByName(const String& n); // Получить тип по имени
	MetaType* getTypeByUID(const TGUID& u);  // Получить тип по УИД
	void fillall(); //
	void add(MetaType* t);
	static void staticTypesLoad(TStream* str);
	int number();
	MetaType* getType(int index);
};


//---------------------------------------------------------------------------
// Значение генерируемого типа
struct GeneratedType
{
	TGUID typeuid; // УИД типа
	TGUID valueuid; // УИД значения
	GeneratedType(tree** ptr, String path);
};

//---------------------------------------------------------------------------
// Предопределенный элемент
struct PredefinedValue
{
	String name; // Имя
	TGUID ref; // Ссылка
	Value1C_metaobj* owner; // Владелец предопределенного значения
	PredefinedValue(const String& n, const TGUID& r, Value1C_metaobj* o) : name(n), ref(r), owner(o){};
	~PredefinedValue(){};
	String getfullname(bool english = false);
};


//---------------------------------------------------------------------------
// Базовый класс значения 1С
class Value1C
{
protected:
	static wchar_t indentstring[31];
public:
	Value1C_obj* parent;
	int index; // Индекс в родителе (-1 - еще не определен, от 0 до (parent->v_objpropv.size() - 1) - именованое свойство, от parent->v_objpropv.size() - элемент коллекции)
	MetaType* type; // Тип значения 1С
	KindOfValue1C kind; // Вид хранимого значения

	Value1C(Value1C_obj* _parent);
	virtual ~Value1C(){};
	virtual String presentation(bool english = false) = 0;
	virtual String valuepresentation(bool english = false) = 0;
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false) = 0;
	virtual bool isempty() = 0;
	String path(MetaContainer* mc, bool english = false);
	String fullpath(MetaContainer* mc, bool english = false);
};

//---------------------------------------------------------------------------
// Значение 1С типа Булево
class Value1C_bool : public Value1C
{
public:
	bool v_bool;    //Булево

	Value1C_bool(Value1C_obj* _parent);
	virtual ~Value1C_bool(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Строка
class Value1C_string : public Value1C
{
public:
	String v_string;

	Value1C_string(Value1C_obj* _parent);
	virtual ~Value1C_string(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Число
class Value1C_number : public Value1C
{
public:
	String v_string;
	int v_number;

	Value1C_number(Value1C_obj* _parent);
	virtual ~Value1C_number(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Число с плавающей запятой
class Value1C_number_exp : public Value1C
{
public:
	String v_string;
	double v_number;

	Value1C_number_exp(Value1C_obj* _parent);
	virtual ~Value1C_number_exp(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Дата
class Value1C_date : public Value1C
{
private:
	static char emptydate[7];
public:
	unsigned char v_date[7];    //Дата

	Value1C_date(Value1C_obj* _parent);
	virtual ~Value1C_date(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа NULL
class Value1C_null : public Value1C
{
public:
	Value1C_null(Value1C_obj* _parent);
	virtual ~Value1C_null(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Неопределено
class Value1C_undef : public Value1C
{
public:
	Value1C_undef(Value1C_obj* _parent);
	virtual ~Value1C_undef(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Тип
class Value1C_type : public Value1C
{
public:
	MetaType* v_type;
	TGUID v_uid;

	Value1C_type(Value1C_obj* _parent);
	virtual ~Value1C_type(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа УникальныйИдентификатор
class Value1C_uid : public Value1C
{
public:
	TGUID v_uid;

	Value1C_uid(Value1C_obj* _parent);
	virtual ~Value1C_uid(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - системное перечисление
class Value1C_enum : public Value1C
{
public:
	MetaValue* v_enum; // Значение системного перечисления

	Value1C_enum(Value1C_obj* _parent);
	virtual ~Value1C_enum(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - стандартный реквизит
class Value1C_stdattr : public Value1C
{
public:
	MetaObject* v_metaobj; // Объект метаданных
	MetaStandartAttribute* v_stdattr;
	int v_value;

	Value1C_stdattr(Value1C_obj* _parent);
	virtual ~Value1C_stdattr(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - стандартная табличная часть
class Value1C_stdtabsec : public Value1C
{
public:
	MetaObject* v_metaobj; // Объект метаданных
	MetaStandartTabularSection* v_stdtabsec;
	int v_value;

	Value1C_stdtabsec(Value1C_obj* _parent);
	virtual ~Value1C_stdtabsec(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С объектного типа
class Value1C_obj : public Value1C
{
protected:
	static bool compare(MetaProperty* p, Value1C* v);
public:
	MetaContainer* owner;
	std::map<MetaProperty*, Value1C*, MetaPropertyLess> v_objprop; //Объект - коллекция свойств
	std::vector<std::pair<MetaProperty*, Value1C*> > v_objpropv; // Коллекция свойств в векторе
	std::vector<Value1C*> v_objcol; //Объект (kv_obj или kv_metaobj???) - коллекция упорядоченных элементов
	std::map<String, int> globalvars; // Коллекция глобальных переменных со значениями
	void fillpropv();

	Value1C_obj(Value1C_obj* _parent, MetaContainer* _owner);
	virtual ~Value1C_obj();
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual Value1C* getproperty(MetaProperty* mp);
	virtual Value1C* getproperty(const String& prop);
//	String path(MetaContainer& mc);
//	String fullpath(MetaContainer& mc);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
	Class* getclass(bool immediately = false);
};

//---------------------------------------------------------------------------
// Значение 1С - объект метаданных
class Value1C_metaobj : public Value1C_obj
{
public:
	MetaObject* v_metaobj;
	std::map<MetaGeneratedType*, GeneratedType*> v_objgentypes; //коллекция генерируемых типов
	std::vector<PredefinedValue*> v_prevalues; // Коллекция предопределенных элементов

	Value1C_metaobj(Value1C_obj* _parent, MetaContainer* _owner);
	virtual ~Value1C_metaobj();
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
//	String path(MetaContainer& mc);
//	String fullpath(MetaContainer& mc);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - ссылка на объект метаданных
class Value1C_refobj : public Value1C
{
public:
	MetaObject* v_metaobj;
	TGUID v_uid;

	Value1C_refobj(Value1C_obj* _parent);
	virtual ~Value1C_refobj(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - ссылка на предопределенное значение
class Value1C_refpre : public Value1C
{
public:
	PredefinedValue* v_prevalue; // Предопределенное значение (kv_refpre)

	Value1C_refpre(Value1C_obj* _parent);
	virtual ~Value1C_refpre(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - ссылка на предопределенное значение
class Value1C_right : public Value1C
{
public:
	MetaRight* v_right; // Право

	Value1C_right(Value1C_obj* _parent);
	virtual ~Value1C_right(){};
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа ДвоичныеДанные
class Value1C_binary : public Value1C
{
public:
	//String v_string; //Имя временного файла
	ExternalFileFormat v_binformat; // Формат двоичных данных
	TTempStream* v_binary; // Двоичные данные

	Value1C_binary(Value1C_obj* _parent);
	virtual ~Value1C_binary();
	virtual String presentation(bool english = false);
	virtual String valuepresentation(bool english = false);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	String get_file_extension();
	virtual bool isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - внешний объект
class Value1C_extobj : public Value1C_obj
{
public:
	bool opened; // признак, открыт ли внешний объект (т.е. распознан, или нет)
	String path; // путь внешнего файла
	TGUID metauid;

	Value1C_extobj(Value1C_obj* _parent, MetaContainer* _owner, const String& _path, MetaType* _type, TGUID _metauid);
	virtual ~Value1C_extobj();

	void open();
	void close();
	virtual Value1C* getproperty(MetaProperty* mp);
	virtual Value1C* getproperty(const String& prop);
	virtual bool Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool isempty();
};


//---------------------------------------------------------------------------
// Значение переменной дерева сериализации
struct VarValue
{
	int value;
	bool isset;
	VarValue(){value = 0; isset = false;};
};

//---------------------------------------------------------------------------
// Структура неинициализированного значения
struct UninitValue1C
{
	Value1C* value; // Неинициализированное значение
	TGUID uid; // УИД значения
	String path; // Путь
	TGUID sauid; // УИД стандартного реквизита
	Value1C_stdtabsec* metats; // Значение стандартной табличной части для стандартного реквизита
	UninitValue1C(Value1C* v, const String& p, const TGUID& u) : value(v), path(p), uid(u){};
	UninitValue1C(Value1C* v, const String& p, const TGUID& u, const TGUID& su, Value1C_stdtabsec* mts) : value(v), path(p), uid(u), sauid(su), metats(mts){};
};

//---------------------------------------------------------------------------
// Контейнер метаданных (конфигурация, внешняя обработка, внешний отчет)
class MetaContainer
{
private:
	Value1C_obj* froot; // корневой объект контейнера
	MetaTypeSet* ftypes; // набор генерируемых типов
	std::map<TGUID, MetaObject*> fmetamap; // Соответствие УИД объектам метаданных
	std::map<String, MetaObject*> fsmetamap; // Соответствие полного имени объектам метаданных (на двух языках)
	std::map<TGUID, PredefinedValue*> fpredefinedvalues; // Соответствие УИД предопределенным элементам
	ContainerVer containerver;
	Version1C ver1C;
	bool useExternal; // Использовать отложенную загрузку внешних файлов

	// ==> Переменные времени загрузки
	String metaprefix;
	std::vector<UninitValue1C> uninitvalues;
	// <== Переменные времени загрузки

	// ==> Переменные времени выгрузки
	unsigned int export_thread_count; // количество потоков экспорта
	Value1C_obj_ExportThread** export_threads; // массив потоков экспорта
	// <== Переменные времени выгрузки

	static TGUID sig_standart_attribute; // сигнатура стандартного реквизита
	static TGUID sig_standart_table_sec; // сигнатура стандартной табличной части
	static TGUID sig_ext_dimension; // сигнатура стандартного реквизита Субконто
	static TGUID sig_ext_dimension_type; // сигнатура стандартного реквизита ВидСубконто

	Value1C* readValue1C(tree** ptr, MetaType* t, Value1C_obj* valparent, const TGUID& metauid, Value1C_stdtabsec*& metats, ClassItem* clitem, String path, bool checkend = false);
	Value1C* readValue1C(MetaType* nt, const SerializationTreeNode* tn, tree*& tr, Value1C_obj* valparent, const TGUID& metauid, Value1C_stdtabsec*& metats, ClassItem* clitem, String& path);
	void recursiveLoadValue1C(Value1C_obj* v, VarValue* varvalues, tree** ptr, const SerializationTreeNode* tn, TGUID& metauid, Value1C_stdtabsec*& metats, ClassItem* clitem, String& path, bool checkend = false);

	int getVarValue(const String& vname, MetaType* t, VarValue* varvalues, ClassItem* clitem, String& path);
	void setVarValue(const String& vname, MetaType* t, Value1C_obj* v, VarValue* varvalues, ClassItem* clitem, int value, String& path);

	void readPredefinedValues(Value1C_metaobj* v, const int ni, const int ui, const Value1C_obj* vStrings, const String& spath);
public:
	ConfigStorage* stor;
	String storpresent;
	static tree* gettree(ConfigStorage* stor, const String& path, bool reporterror = true);
	void loadValue1C(Value1C_obj* v, tree** ptr, const SerializationTreeNode* tn, TGUID metauid, Value1C_stdtabsec* metats, ClassItem* clitem, String& path, bool checkend = false);
	void inituninitvalues();

	MetaContainer(ConfigStorage* _stor, bool _useExternal = false); // Если _useExternal истина, _stor принадлежит MetaContainer и удаляется в деструкторе. Иначе _stor принадлежит вызывающей стороне, и может быть удален сразу после выполнения конструктора
	~MetaContainer();

	MetaObject* getMetaObject(const String& n); // Получить объект метаданных по имени
	MetaObject* getMetaObject(const TGUID& u);  // Получить объект метаданных по УИД
	PredefinedValue* getPreValue(const TGUID& u);  // Получить предопределенный элемент по УИД
	bool Export(const String& path, bool english = false, unsigned int thread_count = 0);
	bool ExportThread(Value1C_obj* v, const String& path, bool english);
	//bool ExportThread(Value1C_obj* v, String path, bool english);

	Value1C_obj* getroot() const { return froot; };
	MetaTypeSet* gettypes() const { return ftypes; };
	std::map<TGUID, MetaObject*> getmetamap() const { return fmetamap; };
	std::map<String, MetaObject*> getsmetamap() const { return fsmetamap; };
	std::map<TGUID, PredefinedValue*> getpredefinedvalues() const { return fpredefinedvalues; };

	// ==> Переменные времени выгрузки
	long export_work_count; // количество заданий выгрузки
	// <== Переменные времени выгрузки

};

//---------------------------------------------------------------------------

class Value1C_obj_ExportThread : public TThread
{
private:
	MetaContainer* owner;
protected:
	void Execute();
public:
	String path;
	bool english;
	Value1C_obj* v;
	long busy; // Признак занятости потока (0 - свободен, 1 - занят)
	TEvent* work; // Сигнал, означающий, что свойства (v, path, english) установлены и нужно сделать экспорт
	bool finish; // признак окончания всех экспортов.
	int singlethread; // счетчик-признак, что текущее задание нужно выполнить в один поток

	Value1C_obj_ExportThread(MetaContainer* _owner);
	~Value1C_obj_ExportThread();

	// Процессно-зависимые свойства MetaContainer
	std::vector<UninitValue1C> uninitvalues;

};

//---------------------------------------------------------------------------

#endif
