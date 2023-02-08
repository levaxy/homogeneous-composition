#include <iostream>
#include<string>
#include<fstream>
#include<vector>
#include"Layer.h"

//#include <cmath>
using namespace std;
int main(){
	ifstream in;
	in.open("Averaging_Model.txt");
	double ContainterVolume = 0;
	double TotalVolume = 0;
	double ConstructionCoef = 0;
	int SizeLayers = 0; // Количество слоёв
	double PuTotalMass = 0;
	double TotalMass = 0;
	double PuAverageConcentration = 0;
	double FillCoef = 0;
	double DispersionZero = 0;
	double DispersionP = 0;
	double Dencity = 0;// как я понял, плотность считаем одинаковой у обоих компонентов
	double SpeedAvgConstant = 0;// Константа скорости смешения
	int FrequenceOptimal = 50;// Частота вращ. оптимальная
	int FrequenceMax = 70;// Частота вращ. крайняя
	int Frequence = 0;
	int AveragingTime = 2000;
	string s;
	getline(in,s);
	in>>ContainterVolume>>Frequence>>ConstructionCoef>>SizeLayers;
	getline(in,s);
	vector<Layer> layers(SizeLayers);
	for(Layer& layer: layers){
		in>>layer.m_Pu>>layer.Mass>>layer.p>>layer.V>>layer.C_Pu;
		PuTotalMass += layer.m_Pu;
		TotalMass += layer.Mass;
		TotalVolume += layer.V;
	}

	PuAverageConcentration = PuTotalMass/TotalVolume;

	FillCoef = TotalVolume/ContainterVolume;

	// Рассчёт константы скорости
	////////////////////////////////////////////////////////////////////////////////////////////////////
	if(Frequence > 0 && Frequence <= FrequenceOptimal)
		SpeedAvgConstant = ConstructionCoef * (1 - FillCoef) * ( (2 * Frequence / FrequenceOptimal) - Frequence*Frequence / (FrequenceOptimal * FrequenceOptimal));
	else if(Frequence > FrequenceOptimal && Frequence <= FrequenceMax)
		SpeedAvgConstant = ConstructionCoef * (1 - FillCoef) * 
		(1 + (2 * Frequence * FrequenceOptimal - Frequence * Frequence - pow(FrequenceOptimal, 2)) / (pow(FrequenceMax - FrequenceOptimal, 2)));
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	Dencity = TotalMass/TotalVolume;

	// Рассчёт дисперсий
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	for(Layer& layer: layers){
		DispersionP += pow(layer.C_Pu - PuAverageConcentration,2)*layer.V;
	}
	DispersionP /= TotalVolume;

	DispersionZero = PuAverageConcentration*(Dencity - PuAverageConcentration);
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	ofstream out;
	out.open("Z(t).csv");
	out<<"t;Z(t);V;n"<<"\n";
	out << 0 << ";" << 0 << ";" << ContainterVolume << ";" << Frequence << ";"  "\n";
	double Z = 0;
	for (size_t t = 1; t < AveragingTime; t++){
		Z = 1 - exp(-SpeedAvgConstant*t);
		out<<t<<";"<<Z<<"\n";
	}
	out.close();

	return 0;
}


