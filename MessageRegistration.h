//---------------------------------------------------------------------------

#ifndef MessageRegistrationH
#define MessageRegistrationH

#include "Common.h"
#include "vcl.h"
#include <vector>
#include <string>

// Глобальный экземпляр регистратора сообщений
extern MessageRegistrator* msreg;

// Функции для инициализации и освобождения регистратора
void InitMessageRegistrator();
void FreeMessageRegistrator();

//---------------------------------------------------------------------------

#endif
