#include"Averaging.h"
//#include<fstream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

WorkParameters:: WorkParameters(int WorkTime, int Frequncy){
    this->WorkTime = WorkTime;
    this->Frequncy = Frequncy;
}
WorkParameters:: WorkParameters(){};
Averaging::Averaging(){
    this->ID = 1;
    this->Name = "Averaging";
    Power = 5;
    ConstructionCoef = 0;// НЕ ЗАБЫТЬ ВВЕСТИ С ФАЙЛА!!!
}
// надо сделать во входном файле в усреднителе поле с вектором структур "Parameters",
// где будет поле с временем работы и с частотой
void Averaging:: SetParams() {
    // чтение вектора с параметрами из json
    QString val;
    QFile Params;
    Params.setFileName("AveragingParams.json");
    Params.open(QIODevice::ReadOnly | QIODevice::Text);
    val = Params.readAll();
    Params.close();

    QJsonDocument JParams  = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = JParams.object();
    QJsonArray params = obj["Params"].toArray();
    for(auto i = params.begin();i < params.end(); i++){
        WorkParameters val(i->toObject()["WorkTime"].toInt(), i->toObject()["Frequncy"].toInt());
        this->VecWorkParams.push_back(val);
    }

    this->CurrentWorkParams = this->VecWorkParams[0];
}

void Averaging:: RiskFunction(QJsonObject& Batch){
    double ContainterVolume = Batch["ContainerVolume"].toDouble();
    double TotalVolume = Batch["TotalVolume"].toDouble();
    int SizeLayers = Batch["SizeLayers"].toInt(); // Количество слоёв
    double PuTotalMass = Batch["PuTotalMass"].toDouble();
    double TotalMass = Batch["TotalMass"].toDouble();
    double PuAverageConcentration;
    double FillCoef;
    double DispersionZero = ;
    double DispersionP = 0;
    double Dencity = 0;// как я понял, плотность считаем одинаковой у обоих компонентов
    double SpeedAvgConstant = 0;// Константа скорости смешения
    int FrequenceOptimal = 50;// Частота вращ. оптимальная
    int FrequenceMax = 70;// Частота вращ. крайняя
    int Frequence = 0;
    int AveragingTime = 2000;

    vector<Layer> layers(SizeLayers);
    for(Layer& layer: layers){
        layer.m_Pu = ;
        layer.Mass;
        layer.p;
        layer.V;
        layer.C_Pu;
        PuTotalMass += layer.m_Pu;
        TotalMass += layer.Mass;
        TotalVolume += layer.V;
    }

    PuAverageConcentration = PuTotalMass/TotalVolume;

    FillCoef = TotalVolume/ContainterVolume;

    // Рассчёт константы скорости
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if(Frequence > 0 && Frequence <= FrequenceOptimal)
        SpeedAvgConstant = ConstructionCoef * (1 - FillCoef) * ( (2 * Frequence / FrequenceOptimal)
                                     - Frequence*Frequence / (FrequenceOptimal * FrequenceOptimal));
    else if(Frequence > FrequenceOptimal && Frequence <= FrequenceMax)
        SpeedAvgConstant = ConstructionCoef * (1 - FillCoef) *
        (1 + (2 * Frequence * FrequenceOptimal - Frequence * Frequence - pow(FrequenceOptimal, 2))
         / (pow(FrequenceMax - FrequenceOptimal, 2)));
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    Dencity = TotalMass/TotalVolume;

    // Рассчёт дисперсий
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    for(Layer& layer: layers){
        DispersionP += pow(layer.C_Pu - PuAverageConcentration,2)*layer.V;
    }
    DispersionP /= TotalVolume;

    DispersionZero = PuAverageConcentration*(Dencity - PuAverageConcentration);
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    ofstream out;
    out.open("Z(t).csv");
    out<<"t;Z(t);V;n"<<"\n";
    out << 0 << ";" << 0 << ";" << ContainterVolume << ";" << Frequence << ";"  "\n";
    double Z = 0;
    for (size_t t = 1; t < AveragingTime; t++){
        Z = 1 - exp(-SpeedAvgConstant*t);
        out<<t<<";"<<Z<<"\n";
    }
    out.close();

}


void Averaging:: Beginner(const int& current_time, Container* cont){
    this->condition = 1;
    this->container = cont;
    this->EndTime = current_time + CurrentWorkParams.WorkTime;
}

void Averaging:: Completer(){
    IterForRunTimes++;
    if(IterForRunTimes >= (int)this->VecWorkParams.size())
        IterForRunTimes = 0;
    this->container->content += 1;
    this->condition = 2;
    this->Motoclock -= this->CurrentWorkParams.WorkTime;
    this->CurrentWorkParams = this->VecWorkParams[IterForRunTimes];
}
