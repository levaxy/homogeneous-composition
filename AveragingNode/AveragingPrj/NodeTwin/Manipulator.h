#pragma once
#include <iostream>
using namespace std;
#include"Container.h"
#include"Operation.h"
#include<vector>
#include<algorithm>
#include"Storage.h"
#include <QString>

class Manipulator
{
public:
	Manipulator();

    int getStatus(size_t t) const;
    double Power;
    double getPower()const;

	Container* container = &NullContainer;
	Operation* Destination;
    Operation* Source;

	int TimeEnd = 0;
	int condition = 0;
	int Motoclock = 0;
	int NewMotorTime;
	int PPR;
	int EndPPR;
	int CurrentWay;
    int ID;
    int IDLocation = 0;
    int SourceID = 0;
    int DestinationID = 0;
    QString Name;
    void Beginner(const int& CurrentTime, Operation*& NewSource, Operation* Newdestination);
	void Completer();
	void ManipManaging(const int& current_time, vector<Operation*>& Objects, vector<int>& queue, Storage* stor);
	void BeginPPR(const int& CurTime);
	void CompletePPR();
	void InitWaysArray();
	bool CheckEndPPR(const int& current_time)const;
    void BeginWayToSourse(const int& CurrentTime);
    void ArriveToSourse(const int& CurrentTime);
private:
	int Gran_averaging;
	int Averaging_prob;
	int Prob_store;
	int Store_AddStZn;
	int AddStZn_averaging;
	int Store_defect_Store;
	int Store_press;
	int Press_gran;
    int Gran_prob;
    int Gran_store;
    int Gran_AddStZn;
    int Averaging_stor;
    int Averaging_press;
    int Prob_AddStZn;
    int Prob_press;
    int AddStZn_press;
	int Ways[8][8];
};
