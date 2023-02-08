#include"Operation.h"
Operation NullOper{};


/*������� ��������� ������������ :
0 - ����� � ���������� ��������
1 - ���������� ��������
2 - �������� ������������
3 - ���*/

Operation:: Operation(){
		this->condition = -1;
		this->EndTime = 0;
		this->CodePrior = 0;
		this->i = 0;
		this->container = &NullContainer;
		this->ID = -1;
		this->EndPPR = 0;
		this->Motoclock = 0;
		this->NewMotorTime = 0;
		this->RunTime = 0;
		this->NextOper = &NullOper;
		this->PPR = 0;

        Power = -1;
        N = 22;
		
}

int Operation:: GetCondit(int flag)const{//flag ����� � ���������
	return this->condition;
}

void Operation:: SetRunTime(const int& runtime, const int& tStart, const int& tend){
	int CountOper = (tend-tStart)/runtime + 1;
	RunTimes.reserve(CountOper);
    for (size_t i = 0; i < CountOper; i++){
		RunTimes.push_back(runtime + rand() % (runtime / 10+1));
	}
	
	RunTime = RunTimes[0];
}

void Operation:: Completer(){//���������� � �� �������, ����� ����������� ������� ��� � ���������, ����� ������ ��������� �� ����
	i++;
	if(i >= RunTimes.size())
		i = 0;
	this->container->content += 1;
	this->condition = 2;
	this->Motoclock -= RunTime;
	this->RunTime = RunTimes[i];
	this->Motoclock-=RunTime;
}

void Operation:: Beginner(const int& current_time, Container* cont){
	this->condition = 1;
	this->container = cont;
	this->EndTime = current_time + RunTime;
}

bool Operation:: CheckReady(){//���������, ����� �� ������, ���� � ���� ������� ���������
		return (this->GetCondit(2) == 2 && this->GetNextOper()->GetCondit(0) == 0);
}

bool Operation:: CheckEnd(const int& current_time)const {
	return (this->EndTime <= current_time && this->condition == 1);
}

Operation* Operation:: GetNextOper() {//��� �������������� � Cell, ���� �� ��������� � ����������� �� �����������
	return this->NextOper->GetThisOper();// � Storage �������������� GetThisOper, ���� ��������� ��������� ������
}

Operation* Operation:: GetThisOper() {
	return this;
}

int Operation :: GetMotorTime(){
	return Motoclock;
}

void Operation::BeginPPR(const int& CurTime){
	this->EndPPR = CurTime + PPR;
	this->condition = 3;
}

void Operation::CompletePPR(){
	this->condition = 0;
	this->Motoclock = NewMotorTime;
}


bool Operation:: CheckEndPPR(const int& current_time)const {
    return (this->EndPPR <= current_time && this->condition == 3);
}

int Operation::getStatus(int currentTime) const
{
    if(CheckEnd(currentTime)){//���� � ��� ������� ��������, �� ������� �������
        return 2;
    }
    else if(CheckEndPPR(currentTime)){
        return 0;
    }
    else{
        return condition;
    }
    return -1;
}

double Operation::getPower() const
{
    if (Power != -1)
        return 0.98*Power + ((double)(rand()%4)/100.0)*Power;
    else return Power;
}

double Operation::getN() const
{
    if (N != -1)
        return 0.9*N + ((double)(rand()%20)/100.0)*N;
    else return N;
}



//� �������� ���������� ������ ���. �������� ���������, EndPPR = currentTime + PPR. � ��� ���� �������� ��������� ���, ��� ����� �������� ��������� �� 0, ��������� ����� �����������.
//� �������� ��� ������ ��������� �� 3, ������� �������� � ������ ������� ������������, �.�. � �������� ������������, ������ ����� � �� ����, �� ��������� ����� ����������� ������ � ���� ��������.
//
