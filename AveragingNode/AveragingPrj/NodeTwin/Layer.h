#pragma once
#include <QtCore/qmetatype.h>
class Layer
{
public:
	Layer();
	double Mass;
	double V;
    double dencity;
	double m_Pu;
	double C_Pu;

private:

};

Q_DECLARE_METATYPE(Layer);
