#include <iostream>
using namespace std;
#include"Container.h"
/*"статусы" контейнера для индикации его места нахождения :
0 - нет контейнера
1 - пустой
2 - с гранулятом
3 - с усреднëнным гранулятом
4 - с качественным гранулятом
5 - с бракованным гранулятом
6 - со стеаратом цинка
7 - с усредненным пресс - порошком
8 - с качественным пресс - порошком
9 - с бракованным пресс - порошком*/


Container:: Container(){
		this->content = 0;
		this->ID = 0;
	}

Container::	Container(const int& content, const int& id){
		this->content = content;
		this->ID = id;
	}



