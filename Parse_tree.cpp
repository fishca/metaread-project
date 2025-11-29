//---------------------------------------------------------------------------

#include "Parse_tree.h"
#include "Common.h"

#include <boost/regex.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//const boost::wregex exp_number(L"^-?[0-9]+\\.?[0-9]*$");
//const boost::wregex exp_number_exp(L"^-?[0-9]+\\.?[0-9]*e-?[0-9]+$");
const boost::wregex exp_number(L"^-?\\d+$");
const boost::wregex exp_number_exp(L"^-?\\d+(\\.?\\d*)?((e|E)-?\\d+)?$");
const boost::wregex exp_guid(L"^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$");
const boost::wregex exp_binary(L"^#base64:[0-9a-zA-Z\\+=\\r\\n\\/]*$");
const boost::wregex exp_binary2(L"^[0-9a-zA-Z\\+=\\r\\n\\/]+$");
const boost::wregex exp_link(L"^[0-9]+:[0-9a-fA-F]{32}$");
const boost::wregex exp_binary_d(L"^#data:[0-9a-zA-Z\\+=\\r\\n\\/]*$");

//---------------------------------------------------------------------------
tree::tree(const String& _value, const node_type _type, tree* _parent)
{
	value = _value;
	type = _type;
	parent = _parent;

	num_subnode = 0;
	index = 0;
	if(parent)
	{
		parent->num_subnode++;
		prev = parent->last;
		if(prev)
		{
			prev->next = this;
			index = prev->index + 1;
		}
		else parent->first = this;
		parent->last = this;
	}
	else prev = NULL;
	next = NULL;
	first = NULL;
	last = NULL;
}

//---------------------------------------------------------------------------
tree::~tree()
{
	while(last) delete last;
	if(prev) prev->next = next;
	if(next) next->prev = prev;
	if(parent)
	{
		if(parent->first == this) parent->first = next;
		if(parent->last == this) parent->last = prev;
		parent->num_subnode--;
	}
}

//---------------------------------------------------------------------------
tree* tree::add_child(const String& _value, const node_type _type)
{
	return new tree(_value, _type, this);
}

//---------------------------------------------------------------------------
tree* tree::add_child()
{
	return new tree(L"", nd_empty, this);
}

//---------------------------------------------------------------------------
tree* tree::add_node()
{
	return new tree(L"", nd_empty, this->parent);
}

//---------------------------------------------------------------------------
String& tree::get_value()
{
	return value;
}

//---------------------------------------------------------------------------
node_type tree::get_type()
{
	return type;
}

//---------------------------------------------------------------------------
void tree::set_value(const String& v, const node_type t)
{
	value = v;
	type = t;
}

//---------------------------------------------------------------------------
int tree::get_num_subnode()
{
	return num_subnode;
}

//---------------------------------------------------------------------------
tree* tree::get_subnode(int _index)
{
	if(_index >= num_subnode) return NULL;
	tree* t = first;
	while(_index)
	{
		t = t->next;
		--_index;
	}
	return t;
}

//---------------------------------------------------------------------------
tree* tree::get_subnode(const String& node_name)
{
	tree* t = first;
	while(t)
	{
		if(t->value == node_name) return t;
		t = t->next;
	}
	return NULL;
}

//---------------------------------------------------------------------------
tree* tree::get_next()
{
	return next;
}

//---------------------------------------------------------------------------
tree* tree::get_parent()
{
	return parent;
}

//---------------------------------------------------------------------------
tree* tree::get_first()
{
	return first;
}

//---------------------------------------------------------------------------
tree& tree::operator [](int _index)
{
	if(!this) return *this; //-V704

	tree* ret = first;
	while(_index)
	{
		if(ret) ret = ret->next;
		--_index;
	}
	return *ret;
}

//---------------------------------------------------------------------------
void tree::outtext(String& text)
{
	node_type lt;
	TReplaceFlags _ReplaceAll;

	if(num_subnode)
	{
		if(text.length()) text += L"\r\n";
		text += L'{';
		tree* t = first;
		while(t)
		{
			t->outtext(text);
			lt = t->type;
			t = t->next;
			if(t) text += L',';
		}
		if(lt == nd_list) text += L"\r\n";
		text += L'}';
	}
	else
	{
		switch(type)
		{
			case nd_string:
				text += L'\"';
				_ReplaceAll << rfReplaceAll;
				text += StringReplace(value, L"\"", L"\"\"", _ReplaceAll);
				text += L'\"';
				break;
			case nd_number:
			case nd_number_exp:
			case nd_guid:
			case nd_list:
			case nd_binary:
			case nd_binary2:
			case nd_link:
			case nd_binary_d:
				text += value;
				break;
			default:
				//if(err) err->AddError(L"Ошибка вывода потока. Пустой или неизвестный узел.");
				break;
		}
	}
}

//---------------------------------------------------------------------------
String tree::path()
{
	String p = L"";
	tree* t;

	if(!this) return L":??"; //-V704
	for(t = this; t->parent; t = t->parent)
	{
		p = String(L":") + std::to_wstring(t->index) + p;
	}
	return p;
}

//---------------------------------------------------------------------------
node_type classification_value(const String& value)
{
	if(value.length() == 0) return nd_empty;
	if(regex_match(value.c_str(), exp_number)) return nd_number;
	if(regex_match(value.c_str(), exp_number_exp)) return nd_number_exp;
	if(regex_match(value.c_str(), exp_guid)) return nd_guid;
	if(regex_match(value.c_str(), exp_binary)) return nd_binary;
	if(regex_match(value.c_str(), exp_link)) return nd_link;
	if(regex_match(value.c_str(), exp_binary2)) return nd_binary2;
	if(regex_match(value.c_str(), exp_binary_d)) return nd_binary_d;
	return nd_unknown;
}

tree* parse_1Cstream(TStream* str, MessageRegistrator* err, const String& path)
{
	TStringBuilder* __curvalue__;

	enum _state{
		s_value, // ожидание начала значения
		s_delimitier, // ожидание разделителя
		s_string, // режим ввода строки
		s_quote_or_endstring, // режим ожидания конца строки или двойной кавычки
		s_nonstring // режим ввода значения не строки
	}state = s_value;

	String curvalue;
	tree* ret;
	tree* t;
	int i;
	wchar_t sym;
	int _sym;
	node_type nt;
	TStreamReader* reader;

	__curvalue__ = new TStringBuilder;

	ret = new tree(L"", nd_list, NULL);
	t = ret;

	//str->Seek(0l, soBeginning);
	reader = new TStreamReader(str, true);

	for(i = 1, _sym = reader->Read(); _sym >= 0; i++, _sym = reader->Read())
	{
		sym = _sym;
		//if(i % 0x100000 == 0) if(err) err->Status(String(i/0x100000) + L" MB");

		switch(state)
		{
			case s_value:
				switch(sym)
				{
					case L' ': // space
					case L'\t':
					case L'\r':
					case L'\n':
						break;
					case L'"':
						//curvalue = L"";
						__curvalue__->Clear();
						state = s_string;
						break;
					case L'{':
						t = new tree(L"", nd_list, t);
						break;
					case L'}':
						if(t->get_first()) t->add_child(L"", nd_empty);
						t = t->get_parent();
						if(!t)
						{
							if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
							        L"Позиция", std::to_wstring(i),
							        L"Путь", path);
							delete ret;
							return NULL;
						}
						state = s_delimitier;
						break;
					case L',':
						t->add_child(L"", nd_empty);
						break;
					default:
						//curvalue = String(sym);
						__curvalue__->Clear();
						__curvalue__->Append(sym);
						state = s_nonstring;
						break;
				}
				break;
			case s_delimitier:
				switch(sym)
				{
					case L' ': // space
					case L'\t':
					case L'\r':
					case L'\n':
						break;
					case L',':
						state = s_value;
						break;
					case L'}':
						t = t->get_parent();
						if(!t)
						{
							if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
							        L"Позиция", std::to_wstring(i),
							        L"Путь", path);
							delete ret;
							return NULL;
						}
						//state = s_delimitier;
						break;
					default:
						if(err) err->AddError(L"Ошибка формата потока. Ошибочный символ в режиме ожидания разделителя.",
						        L"Символ", std::to_wstring(sym),
						        L"Код символа", tohex(sym),
						        L"Путь", path);
						delete ret;
						return NULL;
				}
				break;
			case s_string:
				if(sym == L'"'){
					state = s_quote_or_endstring;
				}
				//else curvalue += String(sym);
				else __curvalue__->Append(sym);
				break;
			case s_quote_or_endstring:
				if(sym == L'"')
				{
					//curvalue += String(sym);
					__curvalue__->Append(sym);
					state = s_string;
				}
				else
				{
					//t->add_child(curvalue, nd_string);
					t->add_child(__curvalue__->ToString(), nd_string);
					switch(sym)
					{
						case L' ': // space
						case L'\t':
						case L'\r':
						case L'\n':
							state = s_delimitier;
							break;
						case L',':
							state = s_value;
							break;
						case L'}':
							t = t->get_parent();
							if(!t)
							{
								if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
								        L"Позиция", std::to_wstring(i),
								        L"Путь", path);
								delete ret;
								return NULL;
							}
							state = s_delimitier;
							break;
						default:
							if(err) err->AddError(L"Ошибка формата потока. Ошибочный символ в режиме ожидания разделителя.",
							        L"Символ", std::to_wstring(sym),
							        L"Код символа", tohex(sym),
							        L"Путь", path);
							delete ret;
							return NULL;
					}
				}
				break;
			case s_nonstring:
				switch(sym)
				{
					case L',':
						curvalue = __curvalue__->ToString();
						nt = classification_value(curvalue);
						if(nt == nd_unknown) if(err) err->AddError(L"Ошибка формата потока. Неизвестный тип значения.",
							L"Значение", curvalue,
							L"Путь", path);
						t->add_child(curvalue, nt);
						state = s_value;
						break;
					case L'}':
						curvalue = __curvalue__->ToString();
						nt = classification_value(curvalue);
						if(nt == nd_unknown) if(err) err->AddError(L"Ошибка формата потока. Неизвестный тип значения.",
							L"Значение", curvalue,
							L"Путь", path);
						t->add_child(curvalue, nt);
						t = t->get_parent();
						if(!t)
						{
							if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
							        L"Позиция", std::to_wstring(i),
							        L"Путь", path);
							delete ret;
							return NULL;
						}
						state = s_delimitier;
						break;
					default:
						//curvalue += String(sym);
						__curvalue__->Append(sym);
						break;
				}
				break;
			default:
				if(err) err->AddError(L"Ошибка формата потока. Неизвестный режим разбора.",
					L"Режим разбора", tohex(state),
					L"Путь", path);
				delete ret;
				return NULL;

		}
	}

	if(state == s_nonstring)
	{
		curvalue = __curvalue__->ToString();
		nt = classification_value(curvalue);
		if(nt == nd_unknown) if(err) err->AddError(L"Ошибка формата потока. Неизвестный тип значения.",
			L"Значение", curvalue,
			L"Путь", path);
		t->add_child(curvalue, nt);
	}
	//else if(state == s_quote_or_endstring) t->add_child(curvalue, nd_string);
	else if(state == s_quote_or_endstring) t->add_child(__curvalue__->ToString(), nd_string);
	else if(state != s_delimitier)
	{
		if(err) err->AddError(L"Ошибка формата потока. Незавершенное значение",
			L"Режим разбора", tohex(state),
			L"Путь", path);
		delete ret;
		return NULL;
	}

	if(t != ret)
	{
		if(err) err->AddError(L"Ошибка формата потока. Не хватает закрывающих скобок } в конце текста разбора.",
			L"Путь", path);
		delete ret;
		return NULL;
	}

	return ret;

}


tree* parse_1Ctext(const String& text, MessageRegistrator* err, const String& path)
{
	TStringBuilder* __curvalue__;

	enum _state{
		s_value, // ожидание начала значения
		s_delimitier, // ожидание разделителя
		s_string, // режим ввода строки
		s_quote_or_endstring, // режим ожидания конца строки или двойной кавычки
		s_nonstring // режим ввода значения не строки
	}state = s_value;

	String curvalue;
	tree* ret;
	tree* t;
	int len = text.length();
	int i;
	wchar_t sym;
	node_type nt;

	__curvalue__ = new TStringBuilder;

	ret = new tree(L"", nd_list, NULL);
	t = ret;

	for(i = 1; i <= len; i++)
	{
		sym = text[i];
		if(!sym) break;

		switch(state)
		{
			case s_value:
				switch(sym)
				{
					case L' ': // space
					case L'\t':
					case L'\r':
					case L'\n':
						break;
					case L'"':
						//curvalue = L"";
						__curvalue__->Clear();
						state = s_string;
						break;
					case L'{':
						t = new tree(L"", nd_list, t);
						break;
					case L'}':
						if(t->get_first()) t->add_child(L"", nd_empty);
						t = t->get_parent();
						if(!t)
						{
							if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
							        L"Позиция", std::to_wstring(i),
							        L"Путь", path);
							delete ret;
							return NULL;
						}
						state = s_delimitier;
						break;
					case L',':
						t->add_child(L"", nd_empty);
						break;
					default:
						//curvalue = String(sym);
						__curvalue__->Clear();
						__curvalue__->Append(sym);
						state = s_nonstring;
						break;
				}
				break;
			case s_delimitier:
				switch(sym)
				{
					case L' ': // space
					case L'\t':
					case L'\r':
					case L'\n':
						break;
					case L',':
						state = s_value;
						break;
					case L'}':
						t = t->get_parent();
						if(!t)
						{
							if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
							        L"Позиция", std::to_wstring(i),
							        L"Путь", path);
							delete ret;
							return NULL;
						}
						//state = s_delimitier;
						break;
					default:
						if(err) err->AddError(L"Ошибка формата потока. Ошибочный символ в режиме ожидания разделителя.",
						        L"Символ", std::to_wstring(sym),
						        L"Код символа", tohex(sym),
						        L"Путь", path);
						delete ret;
						return NULL;
				}
				break;
			case s_string:
				if(sym == L'"'){
					state = s_quote_or_endstring;
				}
				//else curvalue += String(sym);
				else __curvalue__->Append(sym);
				break;
			case s_quote_or_endstring:
				if(sym == L'"')
				{
					//curvalue += String(sym);
					__curvalue__->Append(sym);
					state = s_string;
				}
				else
				{
					//t->add_child(curvalue, nd_string);
					t->add_child(__curvalue__->ToString(), nd_string);
					switch(sym)
					{
						case L' ': // space
						case L'\t':
						case L'\r':
						case L'\n':
							state = s_delimitier;
							break;
						case L',':
							state = s_value;
							break;
						case L'}':
							t = t->get_parent();
							if(!t)
							{
								if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
								        L"Позиция", std::to_wstring(i),
								        L"Путь", path);
								delete ret;
								return NULL;
							}
							state = s_delimitier;
							break;
						default:
							if(err) err->AddError(L"Ошибка формата потока. Ошибочный символ в режиме ожидания разделителя.",
							        L"Символ", std::to_wstring(sym),
							        L"Код символа", tohex(sym),
							        L"Путь", path);
							delete ret;
							return NULL;
					}
				}
				break;
			case s_nonstring:
				switch(sym)
				{
					case L',':
						curvalue = __curvalue__->ToString();
						nt = classification_value(curvalue);
						if(nt == nd_unknown) if(err) err->AddError(L"Ошибка формата потока. Неизвестный тип значения.",
							L"Значение", curvalue,
							L"Путь", path);
						t->add_child(curvalue, nt);
						state = s_value;
						break;
					case L'}':
						curvalue = __curvalue__->ToString();
						nt = classification_value(curvalue);
						if(nt == nd_unknown) if(err) err->AddError(L"Ошибка формата потока. Неизвестный тип значения.",
							L"Значение", curvalue,
							L"Путь", path);
						t->add_child(curvalue, nt);
						t = t->get_parent();
						if(!t)
						{
							if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
							        L"Позиция", std::to_wstring(i),
							        L"Путь", path);
							delete ret;
							return NULL;
						}
						state = s_delimitier;
						break;
					default:
						//curvalue += String(sym);
						__curvalue__->Append(sym);
						break;
				}
				break;
			default:
				if(err) err->AddError(L"Ошибка формата потока. Неизвестный режим разбора.",
					L"Режим разбора", tohex(state),
					L"Путь", path);
				delete ret;
				return NULL;

		}
	}

	if(state == s_nonstring)
	{
		curvalue = __curvalue__->ToString();
		nt = classification_value(curvalue);
		if(nt == nd_unknown) if(err) err->AddError(L"Ошибка формата потока. Неизвестный тип значения.",
			L"Значение", curvalue,
			L"Путь", path);
		t->add_child(curvalue, nt);
	}
	//else if(state == s_quote_or_endstring) t->add_child(curvalue, nd_string);
	else if(state == s_quote_or_endstring) t->add_child(__curvalue__->ToString(), nd_string);
	else if(state != s_delimitier)
	{
		if(err) err->AddError(L"Ошибка формата потока. Незавершенное значение",
			L"Режим разбора", tohex(state),
			L"Путь", path);
		delete ret;
		return NULL;
	}

	if(t != ret)
	{
		if(err) err->AddError(L"Ошибка формата потока. Не хватает закрывающих скобок } в конце текста разбора.",
			L"Путь", path);
		delete ret;
		return NULL;
	}

	return ret;

}

// проверка формата потока
bool test_parse_1Ctext(TStream* str, MessageRegistrator* err, const String& path)
{
	TStringBuilder* __curvalue__;

	enum _state{
		s_value, // ожидание начала значения
		s_delimitier, // ожидание разделителя
		s_string, // режим ввода строки
		s_quote_or_endstring, // режим ожидания конца строки или двойной кавычки
		s_nonstring // режим ввода значения не строки
	}state = s_value;

	String curvalue;
	int i;
	wchar_t sym;
	int _sym;
	node_type nt;
	int level;
	bool ret = true;

	__curvalue__ = new TStringBuilder;

	TStreamReader* reader;

	reader = new TStreamReader(str, true);
	level = 0;

	//for(i = 1, _sym = reader->Read(); _sym >= 0; i++, _sym = reader->Read())
	for(i = 1, _sym = reader->Read(); _sym > 0; i++, _sym = reader->Read())
	{
		sym = _sym;
		//if(i % 0x100000 == 0) if(err) err->Status(String(i/0x100000) + L" MB");

		switch(state)
		{
			case s_value:
				switch(sym)
				{
					case L' ': // space
					case L'\t':
					case L'\r':
					case L'\n':
						break;
					case L'"':
						//curvalue = L"";
						__curvalue__->Clear();
						state = s_string;
						break;
					case L'{':
						level++;
						break;
					case L'}':
						if(level <= 0)
						{
							if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
							        L"Позиция", std::to_wstring(i),
							        L"Путь", path);
							ret = false;
						}
						state = s_delimitier;
						level--;
						break;
					default:
						//curvalue = String(sym);
						__curvalue__->Clear();
						__curvalue__->Append(sym);
						state = s_nonstring;
						break;
				}
				break;
			case s_delimitier:
				switch(sym)
				{
					case L' ': // space
					case L'\t':
					case L'\r':
					case L'\n':
						break;
					case L',':
						state = s_value;
						break;
					case L'}':
						if(level <= 0)
						{
							if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
							        L"Позиция", std::to_wstring(i),
							        L"Путь", path);
							ret = false;
						}
						level--;
						break;
					default:
						if(err) err->AddError(L"Ошибка формата потока. Ошибочный символ в режиме ожидания разделителя.",
						        L"Символ", std::to_wstring(sym),
						        L"Код символа", tohex(sym),
						        L"Путь", path);
						//err->Status(L"");
						delete reader;
						return ret;
				}
				break;
			case s_string:
				if(sym == L'"'){
					state = s_quote_or_endstring;
				}
				//else curvalue += String(sym);
				else __curvalue__->Append(sym);
				break;
			case s_quote_or_endstring:
				if(sym == L'"')
				{
					//curvalue += String(sym);
					__curvalue__->Append(sym);
					state = s_string;
				}
				else
				{
					switch(sym)
					{
						case L' ': // space
						case L'\t':
						case L'\r':
						case L'\n':
							state = s_delimitier;
							break;
						case L',':
							state = s_value;
							break;
						case L'}':
							if(level <= 0)
							{
								if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
								        L"Позиция", std::to_wstring(i),
								        L"Путь", path);
								ret = false;
							}
							level--;
							state = s_delimitier;
							break;
						default:
							if(err) err->AddError(L"Ошибка формата потока. Ошибочный символ в режиме ожидания разделителя.",
							        L"Символ", std::to_wstring(sym),
							        L"Код символа", tohex(sym),
							        L"Путь", path);
							//err->Status(L"");
							delete reader;
							return ret;
					}
				}
				break;
			case s_nonstring:
				switch(sym)
				{
					case L',':
						curvalue = __curvalue__->ToString();
						nt = classification_value(curvalue);
						if(nt == nd_unknown)
						{
							if(err) err->AddError(L"Ошибка формата потока. Неизвестный тип значения.",
								L"Значение", curvalue,
								L"Путь", path);
							ret = false;
						}
						state = s_value;
						break;
					case L'}':
						curvalue = __curvalue__->ToString();
						nt = classification_value(curvalue);
						if(nt == nd_unknown)
						{
							if(err) err->AddError(L"Ошибка формата потока. Неизвестный тип значения.",
								L"Значение", curvalue,
								L"Путь", path);
							ret = false;
						}
						if(level <= 0)
						{
							if(err) err->AddError(L"Ошибка формата потока. Лишняя закрывающая скобка }.",
							        L"Позиция", std::to_wstring(i),
							        L"Путь", path);
							ret = false;
						}
						level--;
						state = s_delimitier;
						break;
					default:
						//curvalue += String(sym);
						__curvalue__->Append(sym);
						break;
				}
				break;
			default:
				if(err) err->AddError(L"Ошибка формата потока. Неизвестный режим разбора.",
					L"Режим разбора", tohex(state),
					L"Путь", path);
				ret = false;
				break;
		}
	}

	if(state == s_nonstring)
	{
		curvalue = __curvalue__->ToString();
		nt = classification_value(curvalue);
		if(nt == nd_unknown)
		{
			if(err) err->AddError(L"Ошибка формата потока. Неизвестный тип значения.",
				L"Значение", curvalue,
				L"Путь", path);
			ret = false;
		}
	}
	else if(state == s_quote_or_endstring)
	{

	}
	else if(state != s_delimitier)
	{
		if(err) err->AddError(L"Ошибка формата потока. Незавершенное значение",
			L"Режим разбора", tohex(state),
			L"Путь", path);
		ret = false;
	}

	if(level > 0)
	{
		if(err) err->AddError(L"Ошибка формата потока. Не хватает закрывающих скобок } в конце текста разбора.",
			L"Путь", path);
		ret = false;
	}

	//err->Status(L"");
	delete reader;
	return ret;

}

String outtext(tree* t)
{
	String text;
	if(t) if(t->get_first()) t->get_first()->outtext(text);
	return text;
}
