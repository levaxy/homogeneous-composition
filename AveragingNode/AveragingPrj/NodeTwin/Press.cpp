#include"Press.h"
//#include"CountTime.h"
Press::	Press() {
		this->ID = 6;
    this->Name = "Press";
    N = -1;
}
void Press::Completer() {//не меняет состояние контейнера, тк выполняет операцию без него
    IterForRunTimes++;
	//this->container->content += 1;
	this->condition = 0;
	this->Motoclock -= RunTime;
    this->RunTime = RunTimes[IterForRunTimes];

}
bool Press:: CheckReady(){//когда проверяем, забирать ли у него контейнер, а делать это мы будем сразу после выгрузки в ту же секунду, когда он начал работу и кондитион у него = 1 и контейнер пустой и он должен отвечать да. 
	//Либо в иные моменты, когда он либо работает, либо ждёт просто, то должен отвечать нет, так как нехрен забирать
    return (this->container->content == 1 && this->NextOper->GetCondit() == 0 && aver->GetCondit()==0);
}
void Press::Beginner(const int& current_time, Container* cont) {
    cont->batch->TimeFabrication = current_time - cont->batch->TimeStartFabrication;
    cont->CalcParams();
	this->condition = 1;
	cont->content = 1;//выгрузили в прессовальню
	this->container = cont;
	this->EndTime = current_time + RunTime;
    this->Batches->append(cont->batch->convertToQJsonObject());
}
int Press::GetCondit()const {//эта вызывается в чекрэди хранилища(гнезда) и надо здесь проверять сразу, свободен ли гранулятор
	if(this->condition == 0 && this->NextOper->condition == 0 && this->aver->condition == 0){
		return 0;
	}
	return 1;
}

