#pragma once
#include <iostream>
using namespace std;
#include"Operation.h"
#include<vector>
#include<queue>
class Laba : public Operation
{
public:
	Laba();
	void Completer() override;
	bool CheckReady() override;
    void Beginner(const int& current_time, Container* cont) override;
private:
	int result = 1;   //1 - <<+>>; 2 - <<->>
    queue<Container*> ProbeQueue;
};
