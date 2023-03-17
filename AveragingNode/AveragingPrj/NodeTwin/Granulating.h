#pragma once
#include <iostream>
using namespace std;
#include"Operation.h"
//#include"Initialisation.h"
class Granulating : public Operation
{
public:
	Granulating();
    void Beginner(const int& current_time, Container* cont) override;
    void Completer() override;
private:
};
