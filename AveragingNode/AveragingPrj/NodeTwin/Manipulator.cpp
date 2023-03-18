#include"Manipulator.h"
#include"NullContainer.h"
Manipulator:: Manipulator(){
    this->container = &NullContainer;
    this->Gran_averaging = 30;
	this->Averaging_prob = 12;
	this->Prob_store = 6;
    this->Store_AddStZn = 6;
    this->AddStZn_averaging = 12;
	this->Store_press = 30;
	this->Press_gran = 50;
    this->Gran_prob = 31;
    this->Gran_store = 20;
    this->Gran_AddStZn = 31;
    this->Averaging_stor = 12;
    this->Averaging_press = 18;
    this->Prob_AddStZn = 0;
    this->Prob_press = 17;
    this->AddStZn_press = 17;
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

double Manipulator::getPower() const
{
    return 0.98*Power + ((double)(rand()%10)/100.0)*Power;
}

// освобождается объект, от куда забираем контейнер и время прибытия в зависимости от пути следования
void Manipulator::Beginner(const int& CurrentTime, Operation*& NewSource,Operation* Newdestination){// Обсудить движение контейнера после пресса
    this->Destination = Newdestination;
    this->Source = NewSource;
    if(NewSource->ID > 10){//ID у гнёзд начинаются с 11, а у операций с 0
        //если это гнездо, то берём ID хранилища. Это чисто для времени пути
        this->SourceID = 4;
    }
    else{
        this->SourceID = NewSource->ID;
    }
    /////////////////
    if (Newdestination->ID > 10) {
        this->DestinationID = 4;
    }
    else {
        this->DestinationID = Newdestination->ID;
    }

    if(this->IDLocation != this->SourceID){
        this->BeginWayToSourse(CurrentTime);
        return;
    }
    if(Source->ID !=6 )//(у пресса не надо трогать кондит. В него высыпали, контейнер сразу повезли на гранулятор, а пресс прессует и позже завершится)
		Source->condition = 0;
    if(Source->Name == "Granulating")
        Source->container->batch->TimeStartFabrication = CurrentTime;
    this->condition = 1;
	this->container = Source->container;
	Source->container = &NullContainer;//присваиваем указатель на нулевой контейнер

//	if(Source->ID == destination->ID){//если с браком, то отвезётся в "блок переработки" и пустымм вернётся в то же гнездо, от куда потом поедет под гранулятор(сейчас такая логика)
//		this->container->content = 1;// выгруз в переработку
//	}

	this->CurrentWay = Ways[SourceID][DestinationID];
	TimeEnd = CurrentTime + CurrentWay; 
	
	if(Source->Motoclock<=0){//Если у сорса моточасы истекли, то вызов метода начала ппр
		Source->BeginPPR(CurrentTime);
	}
}
void Manipulator:: Completer(){
    if(this->IDLocation != this->SourceID){
        ArriveToSourse(this->TimeEnd);
        return;
    }
    this->IDLocation = this->DestinationID;
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
    Ways[0][2] = this->Gran_prob;
    Ways[0][4] = this->Gran_store;
    Ways[0][5] = this->Gran_AddStZn;
    Ways[1][2] = this->Averaging_prob;
    Ways[1][4] = this->Averaging_stor;
    Ways[1][6] = this->Averaging_press;
    Ways[2][4] = this->Prob_store;
    Ways[2][5] = this->Prob_AddStZn;
    Ways[2][6] = this->Prob_press;
    Ways[4][5] = this->Store_AddStZn;
    Ways[4][6] = this->Store_press;
    Ways[5][1] = this->AddStZn_averaging;
    Ways[5][6] = this->AddStZn_press;
    Ways[6][0] = this->Press_gran;

    for(size_t i = 0; i < 6; i++){
        for(size_t j = 0; j < 6; j++){
            if(Ways[i][j] != 0)
                Ways[j][i] = Ways[i][j];
        }
    }
}


void Manipulator::BeginWayToSourse(const int& CurrentTime){
    this->condition = 1;
    this->CurrentWay = Ways[IDLocation][SourceID];
    TimeEnd = CurrentTime + CurrentWay;
}
void Manipulator::ArriveToSourse(const int& CurrentTime){
    this->Motoclock -= CurrentWay;
    this->IDLocation = this->SourceID;
    this->Beginner(CurrentTime, this->Source, this->Destination);
}
