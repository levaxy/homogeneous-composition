#pragma once
#include"IncObj.h"
#include"IncludeJson.h"
#include<vector>
#include<QString>
void AddStatesToJMainArr(const int& t, const Manipulator& manip, const vector<Operation*>& PtrObjects,
                         const vector<Cell*>& PtrCells, QJsonArray& jMainArr);
