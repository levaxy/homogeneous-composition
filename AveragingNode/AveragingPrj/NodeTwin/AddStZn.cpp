#include"AddStZn.h"

AddStZn:: AddStZn(){
this->ID = 5;
    this->Name = "AddStZn";
    Power = 0.5;
}

void AddStZn::Completer() {//вызывается в те моменты, когда завершается процесс или у хранилища, когда пришёл результат из лабы
    IterForRunTimes++;
	this->container->content += 2;
	this->condition = 2;
	this->Motoclock -= RunTime;
    this->RunTime = RunTimes[IterForRunTimes];
    this->Motoclock -= RunTime;
}
int AddStZn:: GetCondit()const{
    if(this->condition == 0 && (probe->condition != 0 || this->aver->condition != 0))
        return 1;
    return this->condition;
}
bool AddStZn:: CheckReady(){//проверяет, готов ли объект, чтоб у него забрали контейнер
        return (this->GetCondit() == 2 && this->GetNextOper()->condition == 0);
}
