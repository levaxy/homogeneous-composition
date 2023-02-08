#include"Averaging.h"


Averaging::Averaging(){
this->ID = 1;
    this->Name = "Averaging";
    Power = 5;
}

void Averaging:: SetRunTime(const int& runtime, const int& tStart, const int& tend) {

	this->RunTimes = {90,95,100,60,90,90,90,90,90,90,90 };//  астомизаци€ времени работы усреднител€

	RunTime = RunTimes[0];
}
