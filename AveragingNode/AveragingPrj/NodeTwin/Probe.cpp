#include"Probe.h"


Probe::	Probe() {
    this->ID = 2;
    this->laba = &NullOper;
    this->Name = "Probe";
    Power = 0.5;
}
void Probe::Completer(){//���������� � �� �������, ����� ����������� ������� ��� � ���������, ����� ������ ��������� �� ����

    this->condition = 2;
    this->RunTime = RunTimes[IterForRunTimes];
    IterForRunTimes++;
    laba->Beginner(this->EndTime, (this->container));
}

bool Probe:: CheckReady() {//���������, ����� �� ������, ���� � ���� ������� ���������
        return (this->GetCondit() == 2 && this->store->CheckFreeCell());
}
int Probe:: GetCondit()const{
    if(this->condition == 0 && addStZn->condition != 0)
        return 1;
    return this->condition;
}
