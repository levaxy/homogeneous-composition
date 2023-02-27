#include"Granulating.h"


Granulating:: Granulating(){
    this->ID = 0;
    this->Name = "Granulating";
    N = -1;
}
void Granulating:: Beginner(const int& current_time, Container* cont){
    cont->TimeStart = current_time;
    this->condition = 1;
    this->container = cont;
    this->EndTime = current_time + RunTime;
}
