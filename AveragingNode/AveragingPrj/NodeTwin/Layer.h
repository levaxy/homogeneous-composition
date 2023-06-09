﻿#pragma once
#include <QtCore/qmetatype.h>
class Layer
{
public:
    Layer();
    Layer(const double& mPu, const double& mass, const double& den, const double& v, const double& Cpu);
    Layer(const Layer& other);
    void operator = (const Layer& other);
    double m_Pu;
    double Mass;
    double dencity;
    double V;
	double C_Pu;

private:

};

//Q_DECLARE_METATYPE(Layer);
