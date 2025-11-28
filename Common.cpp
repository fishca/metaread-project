#include "Common.h"
#include <sstream>
#include <iomanip>
#include <unistd.h>  // for usleep
#include <sched.h>   // for sched_yield

//---------------------------------------------------------------------------
// Реализация базовых функций

bool string_to_GUID(const String& s, TGUID* g) {
	// Заглушка для преобразования строки в GUID
	if (!g) return false;
	
	// Проверяем формат строки и заполняем структуру GUID
	// Пример формата: "03f171e8-326f-41c6-9fa5-932a0b12cddf"
	if (s.length() != 36) return false;
	
	// Заполняем GUID нулями
	memset(g->Data, 0, sizeof(g->Data));
	
	// Просто копируем символы в байты (это заглушка)
	for (size_t i = 0; i < s.length() && i < 16; i++) {
		if (s[i] != L'-') {
			g->Data[i] = (byte)s[i];
		}
	}
	
	return true;
}

String GUID_to_string(const TGUID& g) {
	// Заглушка для преобразования GUID в строку
	std::wostringstream oss;
	oss << std::hex << std::uppercase << std::setfill(L'0');
	oss << std::setw(2) << (int)g.Data[0] << std::setw(2) << (int)g.Data[1] 
		<< std::setw(2) << (int)g.Data[2] << std::setw(2) << (int)g.Data[3] 
		<< L"-" << std::setw(2) << (int)g.Data[4] << std::setw(2) << (int)g.Data[5] 
		<< L"-" << std::setw(2) << (int)g.Data[6] << std::setw(2) << (int)g.Data[7] 
		<< L"-" << std::setw(2) << (int)g.Data[8] << std::setw(2) << (int)g.Data[9] 
		<< L"-" << std::setw(2) << (int)g.Data[10] << std::setw(2) << (int)g.Data[11] 
		<< std::setw(2) << (int)g.Data[12] << std::setw(2) << (int)g.Data[13] 
		<< std::setw(2) << (int)g.Data[14] << std::setw(2) << (int)g.Data[15];
	return oss.str();
}

bool string1C_to_date(const String& s, char* d) {
	// Заглушка для преобразования строки 1С в дату
	if (!d) return false;
	// Просто заполняем дату нулями
	memset(d, 0, 7);
	return true;
}

String date_to_string(const char* d) {
	// Заглушка для преобразования даты в строку
	return L"0001-01-01T00:00:00";
}

String date_to_string1C(const char* d) {
	// Заглушка для преобразования даты в строку 1С
	return L"00010101000000";
}

// Реализация дополнительных функций
String get_node_type_presentation(int node_type) {
    // Заглушка для представления типа узла дерева
    switch (node_type) {
        case 0: return L"nd_min";
        case 1: return L"nd_string";
        case 2: return L"nd_number";
        case 3: return L"nd_number_exp";
        case 4: return L"nd_guid";
        case 5: return L"nd_list";
        case 6: return L"nd_binary";
        case 7: return L"nd_binary2";
        case 8: return L"nd_binary_d";
        default: return L"nd_unknown";
    }
}

void CheckSynchronize(int timeout_ms) {
    // Заглушка для синхронизации потоков (в Linux можно использовать sched_yield или usleep)
    if (timeout_ms > 0) {
        usleep(timeout_ms * 1000); // timeout_ms в миллисекундах
    } else {
        sched_yield(); // уступить процессорное время
    }
}

long InterlockedExchange(long volatile* target, long value) {
    // Атомарная операция обмена значениями
    return __sync_lock_test_and_set(target, value);
}

long InterlockedIncrement(long volatile* target) {
    // Атомарное увеличение на 1
    return __sync_add_and_fetch(target, 1);
}

long InterlockedDecrement(long volatile* target) {
    // Атомарное уменьшение на 1
    return __sync_sub_and_fetch(target, 1);
}

// Определение глобальных переменных
TGUID EmptyUID = {{0,0,0,0}};

//---------------------------------------------------------------------------
