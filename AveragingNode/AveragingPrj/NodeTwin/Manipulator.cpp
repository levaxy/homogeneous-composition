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

int Manipulator::getStatus(size_t t) const// #бесполезная_функция для эмулятора БД
{
    if(TimeEnd <= t && condition == 1){//завершился ли процесс к этому времени
        return 0;
    }
    else if(CheckEndPPR(t)){//закончился ли ППР
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

// освобождается объект, от куда забираем контейнер и время прибытия в зависимости от пути следования
void Manipulator::Beginner(const int& CurrentTime, Operation*& Source,Operation* destination){// Обсудить движение контейнера после пресса
	if(Source->ID !=6 )//(у пресса не надо трогать кондит. В него высыпали, контейнер сразу повезли на гранулятор, а пресс прессует и позже завершится)
		Source->condition = 0;
	this->condition = 1;
	this->container = Source->container;
	Source->container = &NullContainer;//присваиваем указатель на нулевой контейнер
	this->Destination = destination;
	if(Source->ID == destination->ID){//если с браком, то отвезётся в "блок переработки" и пустымм вернётся в то же гнездо, от куда потом поедет под гранулятор(сейчас такая логика)
		this->container->content = 1;// выгруз в переработку
	}
	int SourceID = 0, DestinationID = 0;
	if(Source->ID > 10){//ID у гнёзд начинаются с 11, а у операций с 0
		//если это гнездо, то берём ID хранилища. Это чисто для времени пути 
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
	
	if(Source->Motoclock<=0){//Если у сорса моточасы истекли, то вызов метода начала ппр
		Source->BeginPPR(CurrentTime);
	}
}
void Manipulator:: Completer(){
	this->Destination->Beginner(TimeEnd, this->container);//куда приехал
	this->condition = 0; 
	this->container = &NullContainer;
	this->Motoclock -= CurrentWay;
	if(this->Motoclock <= 0){
		this->BeginPPR(TimeEnd);
	}
	
}
void Manipulator:: ManipManaging(const int& current_time, vector<Operation*>& Objects, vector<int>& queue, Storage* stor) {
	//сначала добавляем в очередь объекты
    for (Operation*& o : Objects) {
        if (o->CheckReady()) { // лаба пропускается, у нее возвр. false
			queue.push_back(o->CodePrior);
		}
	}
//тут по гнездам и если какое готово, то добавить в очередь код приоритетности следующего за ним объекта
	for(Cell*& Cell: stor->Cells){
		try{// Если в гнезде окажется неусреднённый контейнер, то исключение бросается
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
		if(!flag){//flag - если не заняли манипулятор операциями, то хранилище смотрим
			for (Cell*& Cell : stor->Cells) {
				if (Cell->GetNextOper()->CodePrior == queue[0]) {//смотрим какое гнездо подходит по коду приоритетности и для него бегинер 
					Operation* cell = Cell;// бегинер принимает указатель на Operation
					this->Beginner(current_time, cell, Cell->GetNextOper());
					break;
				}
			}
		}
	}

	// у гнезд переопределён метод GetNextOper который по содержимому контейнера определять NextOper
}
void Manipulator::BeginPPR(const int& CurTime) {
	this->EndPPR = CurTime + PPR;
	this->condition = 3;
}
void Manipulator::CompletePPR() {
	this->condition = 0;
	this->Motoclock = NewMotorTime;
}
bool Manipulator::CheckEndPPR(const int& current_time)const {// #бесполезняа_функция для эмулятора БД
	return (this->EndPPR <= current_time && this->condition == 3);
}
void Manipulator::InitWaysArray(){//будет вызываться после заполнения полей
	Ways[0][1] = this->Gran_averaging;
	Ways[1][2] = this->Averaging_prob;
	Ways[2][4] = this->Prob_store;
	Ways[4][5] = this->Store_AddStZn;
	Ways[5][1] = this->AddStZn_averaging;
	Ways[4][4] = this->Store_defect_Store;
	Ways[4][6] = this->Store_press;
	Ways[6][0] = this->Press_gran;
}


