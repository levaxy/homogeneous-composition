#include"AddStZn.h"

AddStZn:: AddStZn(){
this->ID = 5;
    this->Name = "AddStZn";
    Power = 0.5;
}

void AddStZn::Completer() {//���������� � �� �������, ����� ����������� ������� ��� � ���������, ����� ������ ��������� �� ����
    IterForRunTimes++;
	this->container->content += 2;
	this->condition = 2;
	this->Motoclock -= RunTime;
    this->RunTime = RunTimes[IterForRunTimes];
    this->Motoclock -= RunTime;
}
