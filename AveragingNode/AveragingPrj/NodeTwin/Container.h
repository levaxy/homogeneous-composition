#pragma once
#include <iostream>
#include <QList>
#include"Layer.h"
using namespace std;
#include"IncludeJson.h"




class Container
{
public:
	Container();

	Container(const int& content, const int& id);
	int content = 0;
	int ID = 0;
    int TimeStart = 0;
    double Volume = 0;


    class Batch{
    public:
        QList<QVariant>* Layers;
        struct AverageData{
            double FillCoef = 0;
            double M = 0; // Степень смешения
            double Q = 0; // Оценка риска
            int Frequncy = 0;
            int TimeAverage = 0;
        };
        struct PowderingData{
            double FillCoef = 0;
            int Frequncy = 0;
            int TimeAverage = 0;
            double Q = 0; // Оценка риска
        };
        struct DiagramZ_t{
            QVector<double> Z;
            QVector<size_t> t;
        };
        DiagramZ_t Z_t;
        PowderingData Powderingdata;
        AverageData Averagedata;
        int CountAverage = 0;
        int TotalTimeAverage = 0;
        int TimeStartFabrication = 0;
        int TimeFabrication = 0;

        double Dencity = 0; // +
        double TotalMass = 0; // +
        double TotalPuMass = 0; // +
        double TotalVolume = 0; // +
        double PuAverageConcentration = 0;
        QJsonDocument batchToJson(const Batch& batch);

    };
    void SetLayersBatch(QList<QVariant>* layers, const int& TimeArrive);
    Batch* GetBatch();
    friend class Averaging;
    friend class Press;
private:
        Batch *batch = new Batch;

private:

};
