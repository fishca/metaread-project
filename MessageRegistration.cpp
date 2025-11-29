#include "MessageRegistration.h"
#include "vcl.h"
#include <iostream>

// Глобальный экземпляр регистратора сообщений
MessageRegistrator* msreg = nullptr;

// Реализация функций для инициализации и освобождения регистратора
void InitMessageRegistrator() {
    if (!msreg) {
        msreg = new MessageRegistrator();
    }
}

void FreeMessageRegistrator() {
    if (msreg) {
        delete msreg;
        msreg = nullptr;
    }
}