#include"Manipulator.h"
#include"NullContainer.h"
Manipulator:: Manipulator(){
	this->container = &NullContainer;
	this->Gran_averaging = 30;
	this->Averaging_prob = 12;
	this->Prob_store = 6;
	this->Store_AddStZn = 12;
	this->AddStZn_averaging = 15;
	this->Store_defect_Store = 30;
	this->Store_press = 30;
	this->Press_gran = 50;
	for (size_t i = 0; i < 8; i++){
		for (size_t j = 0; j < 8; j++){
			this->Ways[i][j] = 0;
		}
	}
	this->InitWaysArray();
    ID = 100;
    Name = "Manipulator";
    Power = 3;
}

int Manipulator::getStatus(size_t t) const// #�����������_������� ��� ��������� ��
{
    if(TimeEnd <= t && condition == 1){//���������� �� ������� � ����� �������
        return 0;
    }
    else if(CheckEndPPR(t)){//���������� �� ���
        return 0;
    }
    else{
        return condition;
    }
}

double Manipulator::getPower()
{
    return 0.98*Power + ((double)(rand()%10)/100.0)*Power;
}

// ������������� ������, �� ���� �������� ��������� � ����� �������� � ����������� �� ���� ����������
void Manipulator::Beginner(const int& CurrentTime, Operation*& Source,Operation* destination){// �������� �������� ���������� ����� ������
	if(Source->ID !=6 )//(� ������ �� ���� ������� ������. � ���� ��������, ��������� ����� ������� �� ����������, � ����� �������� � ����� ����������)
		Source->condition = 0;
	this->condition = 1;
	this->container = Source->container;
	Source->container = &NullContainer;//����������� ��������� �� ������� ���������
	this->Destination = destination;
	if(Source->ID == destination->ID){//���� � ������, �� �������� � "���� �����������" � ������� ������� � �� �� ������, �� ���� ����� ������ ��� ����������(������ ����� ������)
		this->container->content = 1;// ������ � �����������
	}
	int SourceID = 0, DestinationID = 0;
	if(Source->ID > 10){//ID � ���� ���������� � 11, � � �������� � 0
		//���� ��� ������, �� ���� ID ���������. ��� ����� ��� ������� ���� 
		SourceID = 4;
	}
	else{
		SourceID = Source->ID;
	}
	/////////////////
	if (destination->ID > 10) {
		DestinationID = 4;
	}
	else {
		DestinationID = destination->ID;
	}
	this->CurrentWay = Ways[SourceID][DestinationID];
	TimeEnd = CurrentTime + CurrentWay; 
	
	if(Source->Motoclock<=0){//���� � ����� �������� �������, �� ����� ������ ������ ���
		Source->BeginPPR(CurrentTime);
	}
}
void Manipulator:: Completer(){
	this->Destination->Beginner(TimeEnd, this->container);//���� �������
	this->condition = 0; 
	this->container = &NullContainer;
	this->Motoclock -= CurrentWay;
	if(this->Motoclock <= 0){
		this->BeginPPR(TimeEnd);
	}
	
}
void Manipulator:: ManipManaging(const int& current_time, vector<Operation*>& Objects, vector<int>& queue, Storage* stor) {
	//������� ��������� � ������� �������
    for (Operation*& o : Objects) {
        if (o->CheckReady()) { // ���� ������������, � ��� �����. false
			queue.push_back(o->CodePrior);
		}
	}
//��� �� ������� � ���� ����� ������, �� �������� � ������� ��� �������������� ���������� �� ��� �������
	for(Cell*& Cell: stor->Cells){
		try{// ���� � ������ �������� ������������ ���������, �� ���������� ���������
			if (Cell->CheckReady()) {
				queue.push_back(Cell->GetNextOper()->CodePrior);
			}
		}
		catch (const string ex) {
			cout << ex;
		}
	}

	if (queue.size() > 0) {
		if(queue.size() > 1)
			sort(queue.begin(), queue.end());
		bool flag = false;//
		for (Operation*& obj : Objects) {
			if(obj->CodePrior == queue[0]&&obj->CheckReady()){
				this->Beginner(current_time, obj, obj->GetNextOper());
				flag = true;
				break;
			}
		}//Cell = cell
		if(!flag){//flag - ���� �� ������ ����������� ����������, �� ��������� �������
			for (Cell*& Cell : stor->Cells) {
				if (Cell->GetNextOper()->CodePrior == queue[0]) {//������� ����� ������ �������� �� ���� �������������� � ��� ���� ������� 
					Operation* cell = Cell;// ������� ��������� ��������� �� Operation
					this->Beginner(current_time, cell, Cell->GetNextOper());
					break;
				}
			}
		}
	}

	// � ����� ������������ ����� GetNextOper ������� �� ����������� ���������� ���������� NextOper
}
void Manipulator::BeginPPR(const int& CurTime) {
	this->EndPPR = CurTime + PPR;
	this->condition = 3;
}
void Manipulator::CompletePPR() {
	this->condition = 0;
	this->Motoclock = NewMotorTime;
}
bool Manipulator::CheckEndPPR(const int& current_time)const {// #�����������_������� ��� ��������� ��
	return (this->EndPPR <= current_time && this->condition == 3);
}
void Manipulator::InitWaysArray(){//����� ���������� ����� ���������� �����
	Ways[0][1] = this->Gran_averaging;
	Ways[1][2] = this->Averaging_prob;
	Ways[2][4] = this->Prob_store;
	Ways[4][5] = this->Store_AddStZn;
	Ways[5][1] = this->AddStZn_averaging;
	Ways[4][4] = this->Store_defect_Store;
	Ways[4][6] = this->Store_press;
	Ways[6][0] = this->Press_gran;
}


