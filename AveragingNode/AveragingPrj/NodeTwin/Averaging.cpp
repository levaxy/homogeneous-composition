#include"Averaging.h"
//#include<fstream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
Averaging::Averaging(){
this->ID = 1;
    this->Name = "Averaging";
    Power = 5;
}

void Averaging:: SetRunTime(const int& runtime, const int& tStart, const int& tend) {
    // чтение вектора из json
    QString val;
    QFile RunTimes;
    RunTimes.setFileName("AveragingRunTimes.json");
    RunTimes.open(QIODevice::ReadOnly | QIODevice::Text);
    val = RunTimes.readAll();
    RunTimes.close();

    QJsonDocument JRunTimes  = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = JRunTimes.object();
    QJsonArray a = obj["RunTimes"].toArray();
    for(auto i = a.begin();i < a.end(); i++){
        this->RunTimes.push_back(i->toInt());
    }
    //ifstream ReadRunTimes;
    //ReadRunTimes.open("AveragingRunTimes.json");
//    int t = 0;
//    if(ReadRunTimes.is_open()){
//        while(!ReadRunTimes.eof()){
//            ReadRunTimes >> t;
//            RunTimes.push_back(t);
//        }
//    }
    //this->RunTimes = {90,95,100,60,90,90,90,90,90,90,90 };// Кастомизация времени работы усреднителя

    RunTime = this->RunTimes[0];
}
