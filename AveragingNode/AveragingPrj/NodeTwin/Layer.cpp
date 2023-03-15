#include"Layer.h"

Layer::Layer(){
	Mass = 0;
	V = 0;
    dencity = 0;
	m_Pu = 0;
	C_Pu = 0;
}
Layer::Layer(const double& mPu, const double& mass, const double& den, const double& v, const double& Cpu){
    Mass = mass;
    V = v;
    dencity = den;
    m_Pu = mPu;
    C_Pu = Cpu;
}

Layer::Layer(const Layer& other){
    this->C_Pu = other.C_Pu;
    this->Mass = other.Mass;
    this->V = other.V;
    this->dencity = other.dencity;
    this->m_Pu = other.m_Pu;
}
void Layer::operator = (const Layer& other){
    this->C_Pu = other.C_Pu;
    this->Mass = other.Mass;
    this->V = other.V;
    this->dencity = other.dencity;
    this->m_Pu = other.m_Pu;
}
