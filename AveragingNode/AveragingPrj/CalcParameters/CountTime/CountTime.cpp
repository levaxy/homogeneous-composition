#include <iostream>
#include<map>
#include<fstream>
#include<string>
#include<vector>
using namespace std;
int main()
{
    ifstream ConditInfo;
    string s;
    map<int,int> TimeAveraging, WorkingTime;
    int HowManyCells = 3, current_content = 0;
    int SpecificConsumGood = 0;
    vector<int> Contents(HowManyCells);
    int CurrentTime = 0, status = 0, PreviousStatus = 0, count = 0, Q = 100, SpecificConsum = 0, Mass = 60, CountGood  = 0, TimeStep = 10;



    ConditInfo.open("Out_Condition.txt");
    getline(ConditInfo,s);
    ConditInfo>>CurrentTime;
    ConditInfo.ignore(7);
    ConditInfo>>PreviousStatus;
    ConditInfo.ignore(7);
    for (int& content: Contents){
        ConditInfo>>content;
    }
    while (ConditInfo){
        ConditInfo>>CurrentTime;
        ConditInfo.ignore(7);
        ConditInfo>>status;
        if(PreviousStatus == 1 && status == 1){
            count+=TimeStep;
        }
        else {
            if (count > 0) {
                WorkingTime[CurrentTime] = count + TimeStep;
            }
            TimeAveraging[CurrentTime] = count;
            count = 0;
        }
        PreviousStatus = status;
        ConditInfo.ignore(7);

        for (int& previous_content : Contents) {
            ConditInfo >> current_content;
            if(current_content == 8 && previous_content != 8){
                CountGood++;
            }
            previous_content = current_content;
        }
    }
    cout<<"Count Good: "<<CountGood<<endl<<"\n";

    if(count > 0){
        TimeAveraging[CurrentTime] = count;
        WorkingTime[CurrentTime] = count;
    }


    //cout<<"TimeAveraging: \n\n";
    //for(const auto& TA: TimeAveraging){
    //    cout<<TA.first<<" : "<<TA.second<<endl;
    //}
    int t = 0;


    cout << "WorkingTime: \n\n";
    for (const auto& WT : WorkingTime) {
        cout << WT.first << " : " << WT.second << endl;
        t += WT.second;
    }


    SpecificConsum = t*Q/(Mass*WorkingTime.size());//общее время работы надо разделить на общую массу продукта, но как быть с браком? Если произведённым продуктом считать не брак и брак с любого этапа, то надо во всех гнёздах посчитать, сколько раз встречается подряд идущих 5/9/8

    SpecificConsumGood = t*Q/(Mass*CountGood);// Тут вроде правильно, общее время работы делим на общую массу качественного порошка

    return 0;
}

