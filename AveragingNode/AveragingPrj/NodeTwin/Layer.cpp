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
