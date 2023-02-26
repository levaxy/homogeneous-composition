#include <QJsonArray>
#include<vector>
using namespace std;
template <typename T>
vector<T> FromQJArray_toVector(const QJsonArray& QJArr){
    vector<T> VecFromJArr;
    for(auto i = QJArr.begin();i < QJArr.end(); i++){
        T val = i->toVariant();
        VecFromJArr.push_back(val);
    }
    return VecFromJArr;
}
