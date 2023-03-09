#include <iostream>
using namespace std;
#include"Container.h"
/*"статусы" контейнера для индикации его места нахождения :
0 - нет контейнера
1 - пустой
2 - с гранулятом
3 - с усреднëнным гранулятом
4 - с качественным гранулятом
5 - с бракованным гранулятом
6 - со стеаратом цинка
7 - с усредненным пресс - порошком
8 - с качественным пресс - порошком
9 - с бракованным пресс - порошком*/


Container:: Container(){
}

Container::	Container(const int& content, const int& id){
    this->content = content;
    this->ID = id;
}

QJsonDocument Container::Batch:: batchToJson(const Batch& batch)
{
    QVariantMap map;
    map["Layers"] = QVariant::fromValue(batch.Layers);
    QVariantMap powderingDataMap;
    powderingDataMap["FillCoef"] = batch.Powderingdata.FillCoef;
    //powderingDataMap["ProcessTime"] = batch.Powderingdata.ProcessTime;
    powderingDataMap["Frequncy"] = batch.Powderingdata.Frequncy;
    powderingDataMap["TimeAverage"] = batch.Powderingdata.TimeAverage;
    map["PowderingData"] = QVariant::fromValue(powderingDataMap);
    QVariantMap averageDataMap;
    averageDataMap["FillCoef"] = batch.Averagedata.FillCoef;
    //averageDataMap["ProcessTime"] = batch.Averagedata.ProcessTime;
    averageDataMap["M"] = batch.Averagedata.M;
    averageDataMap["Q"] = batch.Averagedata.Q;
    averageDataMap["Frequncy"] = batch.Averagedata.Frequncy;
    averageDataMap["TimeAverage"] = batch.Averagedata.TimeAverage;
    map["AverageData"] = QVariant::fromValue(averageDataMap);
    map["CountAverage"] = batch.CountAverage;
    map["TotalTimeAverage"] = batch.TotalTimeAverage;
    map["TimeStartFabrication"] = batch.TimeStartFabrication;
    map["TimeFabrication"] = batch.TimeFabrication;
    map["Dencity"] = batch.Dencity;
    map["TotalMass"] = batch.TotalMass;

    return QJsonDocument::fromVariant(map);
}
void Container::SetLayersBatch(QList<QVariant>* layers, const int& TimeArrive){ // Функция для Димы.
    this->batch->Layers = layers;
    this->batch->TimeStartFabrication = TimeArrive;
    for(QVariant layer: *layers){

        batch->TotalPuMass += layer.value<Layer>().m_Pu; // возможно придётся изменить на концентрацию
        batch->TotalMass += layer.value<Layer>().Mass;
        batch->TotalVolume += layer.value<Layer>().Mass/layer.value<Layer>().dencity;
    }
    batch->Dencity = batch->TotalMass/batch->TotalVolume;
    this->batch->PuAverageConcentration = batch->TotalPuMass/batch->TotalMass;
}

Container::Batch* Container::GetBatch(){
    return this->batch;
}





