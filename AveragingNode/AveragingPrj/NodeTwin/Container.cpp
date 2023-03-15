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

QJsonObject Container::Batch:: convertToQJsonObject()
{
    QJsonObject batchObject;
    Batch& batch = *this;
    // Заполнение объекта batchObject данными из объекта batch
    batchObject["Density"] = batch.Dencity;
    batchObject["TotalMass"] = batch.TotalMass;
    batchObject["TotalPuMass"] = batch.TotalPuMass;
    batchObject["TotalVolume"] = batch.TotalVolume;
    batchObject["PuAverageConcentration"] = batch.PuAverageConcentration;
    batchObject["CountAverage"] = batch.CountAverage;
    batchObject["TotalTimeAverage"] = batch.TotalTimeAverage;
    batchObject["TimeStartFabrication"] = batch.TimeStartFabrication;
    batchObject["TimeFabrication"] = batch.TimeFabrication;

    // Преобразование списка слоев в массив объектов
    QJsonArray layersArray;
    foreach (Layer layer, (batch.Layers)) {
        QJsonObject layerObject;
        layerObject["m_Pu"] = layer.m_Pu;
        layerObject["Mass"] = layer.Mass;
        layerObject["Density"] = layer.dencity;
        layerObject["V"] = layer.V;
        layerObject["C_Pu"] = layer.C_Pu;
        layersArray.append(layerObject);
    }
    batchObject["Layers"] = layersArray;

    // Добавление данных из объектов AverageData и PowderingData
    QJsonObject averageDataObject;
    averageDataObject["FillCoef"] = batch.Averagedata.FillCoef;
    averageDataObject["M"] = batch.Averagedata.M;
    averageDataObject["Q"] = batch.Averagedata.Q;
    averageDataObject["Frequncy"] = batch.Averagedata.Frequncy;
    averageDataObject["TimeAverage"] = batch.Averagedata.TimeAverage;
    batchObject["AverageData"] = averageDataObject;

    QJsonObject powderingDataObject;
    powderingDataObject["FillCoef"] = batch.Powderingdata.FillCoef;
    powderingDataObject["Frequncy"] = batch.Powderingdata.Frequncy;
    powderingDataObject["TimeAverage"] = batch.Powderingdata.TimeAverage;
    powderingDataObject["Q"] = batch.Powderingdata.Q;
    batchObject["PowderingData"] = powderingDataObject;

    // Добавление данных из объекта DiagramZ_t
    QJsonObject diagramObject;
    QJsonArray zArray, tArray;
    for (int i = 0; i < batch.Z_t.Z.size(); ++i) {
        zArray.append(batch.Z_t.Z[i]);
        tArray.append(static_cast<int>(batch.Z_t.t[i]));
    }
    diagramObject["Z"] = zArray;
    diagramObject["t"] = tArray;
    batchObject["DiagramZ_t"] = diagramObject;

    // Создание объекта QJsonDocument из полученного QJsonObject
    return batchObject;
}
void Container::SetLayersBatch(QList<Layer> layers){ // Функция для Димы.
    this->batch->Layers = layers;
    for(Layer layer: layers){

        batch->TotalPuMass += layer.m_Pu; // возможно придётся изменить на концентрацию
        batch->TotalMass += layer.Mass;
        batch->TotalVolume += layer.Mass/layer.dencity;
    }
    batch->Dencity = batch->TotalMass/batch->TotalVolume;
    this->batch->PuAverageConcentration = batch->TotalPuMass/batch->TotalMass;
}

Container::Batch* Container::GetBatch(){
    return this->batch;
}

void Container:: CalcParams(){
    int Q = 100;
    double Mass = this->batch->TotalMass;
    Container::TotalTime += this->batch->TimeFabrication;
    Container::CountContainers++;
    if(this->content == 8)
        Container::CountGoodContainers++;
    Container::SpecificConsum = TotalTime*Q/(Mass*CountContainers);//общее время работы разделить на общую массу продукта

    Container::SpecificConsumGood = TotalTime*Q/(Mass*CountGoodContainers);// Общее время работы делим на общую массу качественного порошка

}

int Container::TotalTime = 0;
int Container::CountContainers = 0;
int Container::CountGoodContainers = 0;
double Container:: Container::SpecificConsum = 0;
double Container::SpecificConsumGood = 0;

