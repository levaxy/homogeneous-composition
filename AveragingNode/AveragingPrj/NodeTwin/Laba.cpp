#include"Laba.h"
#include"NullContainer.h"

Laba:: Laba(){
		this->ID = 3;// ЗДЕСЬ ИЗМЕНЕНИЯ. первые два - брак, остальные качеств.
		// В Laba.h появился results
		this->r = 0;
		results = {2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        Name = "Lab";
        Power = 0.5;
	}
void Laba::Completer() {//
	//if((double)rand()/RAND_MAX < 0.8)
	//	this->result = 1;
	//else
	//	this->result = 2;
    if(r == results.size())// всегда положительный результат
		results.push_back(1);
	result = results[r];
	r++;
	this->container->content += this->result;
	this->RunTime = this->RunTimes[i];
	i++;
    ProbeQueue.erase(next(ProbeQueue.begin()));
    if(ProbeQueue.empty()){
        this->condition = 0;
        this->container = &NullContainer;
    }
    else{
        this->container = ProbeQueue[0];
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
    ProbeQueue.push_back(cont);
}


