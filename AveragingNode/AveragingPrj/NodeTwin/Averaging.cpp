#include"Averaging.h"
#include<fstream>

Averaging::Averaging(){
this->ID = 1;
    this->Name = "Averaging";
    Power = 5;
}

void Averaging:: SetRunTime(const int& runtime, const int& tStart, const int& tend) {
    ifstream ReadRunTimes;
    ReadRunTimes.open("AveratingRunTimes.txt");
    int t = 0;
    if(ReadRunTimes.is_open()){
        while(!ReadRunTimes.eof()){
            ReadRunTimes >> t;
            RunTimes.push_back(t);
        }
    }
    //this->RunTimes = {90,95,100,60,90,90,90,90,90,90,90 };//  астомизаци€ времени работы усреднител€

	RunTime = RunTimes[0];
}
