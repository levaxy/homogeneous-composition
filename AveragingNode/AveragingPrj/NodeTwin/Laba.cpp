#include"Laba.h"
#include"NullContainer.h"


Laba:: Laba(){
        this->ID = 3;

        Name = "Lab";
        Power = 0.5;
	}
void Laba::Completer() {//
    if(this->container->GetBatch()->CountAverage == 1){
        if(this->container->GetBatch()->Averagedata.Q < 25)
            this->result = 1;
        else
            this->result = 2;
    }
    else{
        if(this->container->GetBatch()->Powderingdata.Q < 25)
            this->result = 1;
        else
            this->result = 2;
    }
	this->container->content += this->result;
    this->RunTime = this->RunTimes[IterForRunTimes];
    IterForRunTimes++;
    ProbeQueue.pop();
    if(ProbeQueue.empty()){
        this->condition = 0;
        this->container = &NullContainer;
    }
    else{
        this->container = ProbeQueue.front();
        this->EndTime = EndTime + RunTime;
    }
}

bool Laba::CheckReady() {//лаба не подаёт запрос
	return (false);
}
void Laba::Beginner(const int& current_time, Container* cont){//
    if(ProbeQueue.empty()){
        this->EndTime = current_time + RunTime;
        this->container = cont;
        this->condition = 1;
    }
    ProbeQueue.push(cont);
}


