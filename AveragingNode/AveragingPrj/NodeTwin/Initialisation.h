#pragma once
#include"IncludeJson.h"
#include"IncObj.h"
#include<string>
#include<iostream>
using namespace std;

QJsonObject ReadJson(const QString& FileName);
void InitOperations(const QJsonObject& AllData, vector<Operation*>& Operations, vector<Container*>& Containers);
vector<Cell> InitCells(const QJsonObject& AllData, vector<Container*>& Containers);
vector<Container> InitContainers(const QJsonObject& AllData);
Manipulator InitManipulator(const QJsonObject& AllData, vector<Operation*>& Operations, vector<Cell*>& Cells);
