#include"Granulating.h"


Granulating:: Granulating(){
    this->ID = 0;
    this->Name = "Granulating";
    N = -1;
}
void Granulating:: Beginner(const int& current_time, Container* cont){
    cont->TimeStart = current_time;
    this->condition = 1;
    this->container = cont;
    this->EndTime = current_time + RunTime;
    QList<Layer> layers;
//m_Pu, кг	m_слоя, кг	ρ_слоя, г/см3	V_слоя, л	С_Pu_в_слое, кг/л
    layers.append(Layer(0.187, 1.1, 6.0, 0.183, 1.02));
    layers.append(Layer(0.136, 0.8, 5.8, 0.138, 0.986));
    layers.append(Layer(0.132, 0.6, 6.0, 0.1, 1.32));
    layers.append(Layer(0.18, 1.0, 5.7, 0.175, 1.026));
    layers.append(Layer(0.3, 1.2, 6.0, 0.2, 1.5));
    layers.append(Layer(0.119, 0.7, 5.8, 0.121, 0.986));
    layers.append(Layer(0.153, 0.9, 5.9, 0.153, 1.003));
    layers.append(Layer(0.33, 1.5, 5.9, 0.254, 1.298));
    layers.append(Layer(0.36, 1.5, 5.8, 0.259, 1.392));
    layers.append(Layer(0.16, 0.8, 5.6, 0.143, 1.12));
    layers.append(Layer(0.165, 1.1, 5.9, 0.186, 0.885));
    layers.append(Layer(0.144, 0.9, 5.8, 0.155, 0.928));
    layers.append(Layer(0.22, 1.1, 5.6, 0.196, 1.12));
    layers.append(Layer(0.175, 0.7, 5.5, 0.127, 1.375));
    layers.append(Layer(0.176, 1.1, 5.6, 0.196, 0.896));
    layers.append(Layer(0.17, 1.0, 5.9, 0.169, 1.003));
    layers.append(Layer(0.336, 1.4, 5.8, 0.241, 1.392));
    layers.append(Layer(0.36, 1.5, 5.8, 0.259, 1.392));
    layers.append(Layer(0.25, 1.0, 5.9, 0.169, 1.475));
    layers.append(Layer(0.21, 1.0, 6.0, 0.167, 1.26));
    layers.append(Layer(0.24, 1.2, 5.9, 0.203, 1.18));
    layers.append(Layer(0.175, 0.7, 5.9, 0.119, 1.475));
    layers.append(Layer(0.252, 1.2, 5.6, 0.214, 1.176));
    layers.append(Layer(0.198, 1.100, 5.800, 0.190, 1.044));
    layers.append(Layer(0.336, 1.400, 5.700, 0.246, 1.368));
    layers.append(Layer(0.120, 0.800, 5.900, 0.136, 0.885));
    layers.append(Layer(0.168, 0.800, 5.800, 0.138, 1.218));
    layers.append(Layer(0.144, 0.600, 5.800, 0.103, 1.392));
    layers.append(Layer(0.204, 1.200, 5.500, 0.218, 0.935));
    layers.append(Layer(0.120, 0.800, 5.600, 0.143, 0.840));
    layers.append(Layer(0.234, 1.300, 5.700, 0.228, 1.026));
    layers.append(Layer(0.192, 0.800, 5.900, 0.136, 1.416));
    layers.append(Layer(0.240, 1.500, 6.000, 0.250, 0.960));
    layers.append(Layer(0.247, 1.300, 6.000, 0.217, 1.140));
    layers.append(Layer(0.200, 1.000, 5.600, 0.179, 1.120));
    layers.append(Layer(0.184, 0.800, 5.900, 0.136, 1.357));
    layers.append(Layer(0.144, 0.800, 5.900, 0.136, 1.062));
    layers.append(Layer(0.285, 1.500, 5.800, 0.259, 1.102));
    layers.append(Layer(0.264, 1.100, 5.500, 0.200, 1.320));
    layers.append(Layer(0.264, 1.100, 5.900, 0.186, 1.416));
    layers.append(Layer(0.216, 1.200, 5.800, 0.207, 1.044));
    layers.append(Layer(0.153, 0.900, 6.000, 0.150, 1.020));


    cont->SetLayersBatch(layers, current_time);
}
