#pragma once
#include <iostream>
using namespace std;
#include"Operation.h"

class AddStZn : public Operation
{
public:
	AddStZn();
	void Completer()override;
    int GetCondit() const override;
    bool  CheckReady() override;
    Operation* probe;
    Operation* aver;
private:

};
