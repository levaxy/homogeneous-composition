#pragma once
#include <iostream>
using namespace std;
#include"Operation.h"

class Probe : public Operation
{
public:
	Probe();
	void Completer()override ;
	int GetCondit(int flag)const;//���� ���� ������, �� � ������ ����� ������(������ ������) �� �� �����. ������� ��� ������ � �� ��������
//	Operation* GetNextOper();
	Operation* laba;
};
//��������, �� ������ ����� �������� 
//���� � ����� ��������� ����� ��� ������, �� ���� �������������� ����� ������������ NextOper ������