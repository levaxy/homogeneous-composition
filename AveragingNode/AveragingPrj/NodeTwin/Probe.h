#pragma once
#include <iostream>
using namespace std;
#include"Operation.h"
#include"Storage.h"
class Probe : public Operation
{
public:
	Probe();
	void Completer()override ;
    bool CheckReady() override;
    Operation* laba;
    Storage* store;
    //	Operation* GetNextOper();
};
