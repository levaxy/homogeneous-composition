#pragma once
#include <iostream>
using namespace std;
#include"IncludeJson.h"
class Container
{
public:
	Container();

	Container(const int& content, const int& id);
	int content = 0;
	int ID = 0;
    int TimeStart = 0;
    QJsonObject Layer;
private:

};
