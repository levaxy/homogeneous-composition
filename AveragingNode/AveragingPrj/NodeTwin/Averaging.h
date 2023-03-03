﻿#pragma once
#include <iostream>
using namespace std;
#include"Operation.h"
#include"Container.h"
#include"Layer.h"
#include<cmath>

struct WorkParameters{
    WorkParameters(int WorkTime, int Frequncy);
    WorkParameters();
    int WorkTime = 0;
    int Frequncy = 0;
};

class Averaging : public Operation
{
public:
	Averaging();
    void SetParams();
    void RiskFunction(QJsonObject& Batch);
    void Beginner(const int& current_time, Container* cont) override;
    void Completer() override;
    double ConstructionCoef;
    WorkParameters CurrentWorkParams;
    vector<WorkParameters> VecWorkParams;
};
