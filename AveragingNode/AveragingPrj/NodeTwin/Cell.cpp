#include"Cell.h"

//���� 
Cell:: Cell():Operation(){//����� ��������� �� ��������� � ���� ������, ������ ���������� ����������, ����������� � ������
    this->addstzn = &NullOper;
    this->granulator = &NullOper;
    this->press = &NullOper;
    this->Motoclock = 1;// �.�. � ������������ �� ��������� RunTime = 0, �� �������� �� ����� ����������� � ������ � ����������
    this->CodePrior = 10;// ��� ��������� ���� �������������� � managing_manip ���� ����, �� ����� ��� ����� ����������� � ������� ��������.(��� ��� �������������� ����� �� �����������). ���� ������� ������ ������, ��� ���� ����� �� �����, �� ����� ������ ������ ���� ���, ��������� � ������ ���������� � ������� � �� �� �����.
    this->Name = "Cell";
    N = 22/3;
}

Operation* Cell:: GetNextOper(){
    if(this->container->content == 1){//���� ������ ���������, �� ��������� ����� - ��� ����������
        return this->granulator;
    }
    if (this->container->content == 4) {//���, �� ����, �� � �����. �����
        return addstzn;
    }
    if (this->container->content == 9 || this->container->content == 8 || this->container->content == 5) {//���� �������, ���� ���� �� stz �� �� �����
        return press;
    }
    return &NullOper;//� ���� ������� ��������� �������� ���
}

bool Cell::CheckReady() {
    return (this->GetCondit() == 2 && this->GetNextOper()->GetCondit() == 0);
}

void Cell:: Beginner(const int& current_time, Container* cont){
    this->container = cont;
}

int Cell::GetCondit()const{
    if(this->container->content == 0){
        return 0;
    }
    else if(this->container->content == 7 || this->container->content == 3){
        return 1;
    }
    else if(this->container->content == 2 || this->container->content == 6){
        throw("������������ ������� ����� � ���������!\n");
    }
    else{
        return 2;
    }
}
