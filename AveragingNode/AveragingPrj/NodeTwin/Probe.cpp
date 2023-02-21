#include"Probe.h"


Probe::	Probe() {
    this->ID = 2;
    this->laba = &NullOper;
    this->Name = "Probe";
    Power = 0.5;
}
void Probe::Completer(){//вызывается в те моменты, когда завершается процесс или у хранилища, когда пришёл результат из лабы

    this->condition = 2;
    this->RunTime = RunTimes[i];
    i++;
    laba->Beginner(this->EndTime, (this->container));
}

bool Probe:: CheckReady() {//проверяет, готов ли объект, чтоб у него забрали контейнер
        return (this->GetCondit() == 2 && this->store->CheckFreeCell());
}
