#include"Press.h"

Press::	Press() {
		this->ID = 6;
    this->Name = "Press";
    N = -1;
}
void Press::Completer() {//�� ������ ��������� ����������, �� ��������� �������� ��� ����
	i++;
	//this->container->content += 1;
	this->condition = 0;
	this->Motoclock -= RunTime;
	this->RunTime = RunTimes[i];

}
bool Press:: CheckReady(){//����� ���������, �������� �� � ���� ���������, � ������ ��� �� ����� ����� ����� �������� � �� �� �������, ����� �� ����� ������ � ��������� � ���� = 1 � ��������� ������ � �� ������ �������� ��. 
	//���� � ���� �������, ����� �� ���� ��������, ���� ��� ������, �� ������ �������� ���, ��� ��� ������ ��������
    return (this->container->content == 1 && this->NextOper->GetCondit() == 0 && aver->GetCondit()==0);
}
void Press::Beginner(const int& current_time, Container* cont) {
	this->condition = 1;
	cont->content = 1;//��������� � ������������
	this->container = cont;
	this->EndTime = current_time + RunTime;

}
int Press::GetCondit()const {//��� ���������� � ������� ���������(������) � ���� ����� ��������� �����, �������� �� ����������
	if(this->condition == 0 && this->NextOper->condition == 0 && this->aver->condition == 0){
		return 0;
	}
	return 1;
}

