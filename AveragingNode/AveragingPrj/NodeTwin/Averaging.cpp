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
    ConstructionCoef = 0;
}

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
    this->ConstructionCoef = obj["ConstructionCoef"].toDouble();
    QJsonArray params = obj["WorkParameters"].toArray();
    for(auto i = params.begin();i < params.end(); i++){
        WorkParameters val(i->toObject()["WorkTime"].toInt(), i->toObject()["Frequncy"].toInt());
        this->VecWorkParams.push_back(val);
    }

    this->CurrentWorkParams = this->VecWorkParams[0];

}
// заглушка вместо модели опудривания
void Averaging::PowderingRiskFunction(Container& cont){
    cont.batch->Powderingdata.FillCoef = cont.batch->Averagedata.FillCoef;
    cont.batch->Powderingdata.Frequncy = this->CurrentWorkParams.Frequncy;
    cont.batch->Powderingdata.TimeAverage = this->CurrentWorkParams.WorkTime;
    cont.batch->Powderingdata.Q = 10;
    cont.batch->CountAverage++;
    cont.batch->TotalTimeAverage += this->CurrentWorkParams.WorkTime;
}
void Averaging:: AverageRiskFunction(Container& cont){
    //double ContainerVolume = cont.Volume;
    double TotalVolume = cont.GetBatch()->TotalVolume;
    int SizeLayers = cont.GetBatch()->Layers.size(); // Количество слоёв
    double PuTotalMass = cont.GetBatch()->TotalPuMass;
    double TotalMass = cont.GetBatch()->TotalMass;
    double PuAverageConcentration = cont.batch->PuAverageConcentration;;
    double FillCoef = TotalVolume/cont.Volume;

    double FrequenceOptimal = 50;// Частота вращ. оптимальная
    double FrequenceMax = 70;// Частота вращ. крайняя
    double Frequency = this->CurrentWorkParams.Frequncy;
    double AveragingTime = this->CurrentWorkParams.WorkTime;
    cont.batch->CountAverage++;
    cont.batch->Averagedata.FillCoef = FillCoef;
    cont.batch->Averagedata.Frequncy = Frequency;
    cont.batch->Averagedata.TimeAverage = AveragingTime;
    // Рассчёт константы скорости
    double SpeedAvgConstant = 0;
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// // при частоте вращения равной максимальной конст. скорости смешения получается равной нулю
    if(Frequency > 0 && Frequency <= FrequenceOptimal)
        SpeedAvgConstant = ConstructionCoef * (1.0 - FillCoef) * ( (2.0 * Frequency / FrequenceOptimal)
                                     - Frequency*Frequency / (FrequenceOptimal * FrequenceOptimal));
    else if(Frequency > FrequenceOptimal && Frequency <= FrequenceMax)
        SpeedAvgConstant = ConstructionCoef * (1.0 - FillCoef) *
                (1.0 + (2.0 * Frequency * FrequenceOptimal - Frequency * Frequency -
                        pow(FrequenceOptimal, 2.0)) / (pow(FrequenceMax - FrequenceOptimal, 2.0)));
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    double Dencity = cont.batch->Dencity;
    // Рассчёт дисперсий
    double DispersionP = 0;
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    for(Layer& layer: cont.batch->Layers){
        DispersionP += pow(layer.C_Pu - PuAverageConcentration,2)*(layer.Mass/layer.dencity);
    }
    DispersionP /= TotalVolume;

    double DispersionZero = PuAverageConcentration*(Dencity - PuAverageConcentration);
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    QVector<double> Z(AveragingTime);
    QVector<size_t> time(AveragingTime);
    for (size_t t = 1; t <= AveragingTime; t++){
        Z[t-1] = 1 - exp(-SpeedAvgConstant*t);
        time[t-1] = t;
    }
    cont.batch->Z_t.Z = Z; // вставили график Z(t) в структуру
    cont.batch->Z_t.t = time;
    cont.batch->Averagedata.M = Z.back();
    cont.batch->TotalTimeAverage += AveragingTime;
    cont.batch->Averagedata.Q = (0.32*abs(0.2 - FillCoef)/0.2 + 0.34*abs(FrequenceOptimal - Frequency)
                                 / FrequenceOptimal + 0.34*abs(900 - AveragingTime*10)/900)*100;
}


void Averaging:: Beginner(const int& current_time, Container* cont){
    this->condition = 1;
    this->container = cont;
    this->EndTime = current_time + CurrentWorkParams.WorkTime;
    if(cont->batch->CountAverage == 0)
        AverageRiskFunction(*cont);
    else
        PowderingRiskFunction(*cont);
    //cont->batch->CountAverage++;
    //cont->batch->TotalTimeAverage += CurrentWorkParams.WorkTime;
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
