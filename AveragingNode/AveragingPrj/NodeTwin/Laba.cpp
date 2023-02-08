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
	if(r == results.size())
		results.push_back(1);
	result = results[r];
	r++;
	this->container->content += this->result;
	this->RunTime = this->RunTimes[i];
	i++;
	this->condition = 0;
	this->container = &NullContainer;
}

bool Laba::CheckReady() {//лаба не подаёт запрос
	return (false);
}



