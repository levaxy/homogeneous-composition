#include"CountTime.h"
#include"VarCountTime.h"
void CalcParams(const int& TimeInProcess, const int& Condition){
    int Q = 100, Mass = 60;
    TotalTime += TimeInProcess;
    CountContainers++;
    if(Condition == 8)
        CountGoodContainers++;
    SpecificConsum = TotalTime*Q/(Mass*CountContainers);//общее время работы надо разделить на общую массу продукта, но как быть с браком? Если произведённым продуктом считать не брак и брак с любого этапа, то надо во всех гнёздах посчитать, сколько раз встречается подряд идущих 5/9/8

    SpecificConsumGood = TotalTime*Q/(Mass*CountGoodContainers);// Тут вроде правильно, общее время работы делим на общую массу качественного порошка

}

