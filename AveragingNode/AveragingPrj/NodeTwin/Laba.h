#pragma once
#include <iostream>
using namespace std;
#include"Operation.h"
#include<vector>

class Laba : public Operation
{
public:
	Laba();
	void Completer() override;
	bool CheckReady() override;
    void Beginner(const int& current_time, Container* cont) override;
private:
	int result = 1;   //1 - <<+>>; 2 - <<->>
	vector<int> results;
    vector<Container*> ProbeQueue;
    int r;// счётчик
};
