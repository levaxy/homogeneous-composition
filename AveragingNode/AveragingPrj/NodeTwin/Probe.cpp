#include"Probe.h"


Probe::	Probe() {
    this->ID = 2;
    this->laba = &NullOper;
    this->Name = "Probe";
    Power = 0.5;
}
void Probe::Completer(){//���������� � �� �������, ����� ����������� ������� ��� � ���������, ����� ������ ��������� �� ����

    this->condition = 2;
    this->RunTime = RunTimes[i];
    i++;
    laba->Beginner(this->EndTime, (this->container));
}

bool Probe:: CheckReady() {//���������, ����� �� ������, ���� � ���� ������� ���������
        return (this->GetCondit() == 2 && this->store->CheckFreeCell());
}
