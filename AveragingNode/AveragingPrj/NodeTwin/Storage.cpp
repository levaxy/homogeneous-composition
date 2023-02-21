#include"Storage.h"

Storage::Storage() {
    this->ID = 4;
    this->Name = "Storage";
    Power = 1;
}
void Storage::SetCells(vector<Cell*>& Cells){
    this->Cells = Cells;
}
//Operation* Storage::GetNextOper() {
//    return this->NextOper;
//}
bool Storage::IsRequester(){// ������, ���� �� ������, �������� ������
    for (const Operation* cell : this->Cells) {
        if (cell->GetCondit() == 2) {
            return true;
        }
    }
    return false;
}
bool Storage::CheckFreeCell(){// ������, ���� ��������� ������
    for(const Operation* obj: this->Cells){
        if(obj->GetCondit() == 0){
            return true;
        }
    }
    return false;
}

Operation* Storage::GetThisOper(){
    for(Cell*& cell: Cells){
        if (cell->GetCondit() == 0) {
            return cell;
        }
    }
    return &NullOper;
}

bool Storage::CheckEnd(const int& current_time)const {//���� ���������� � ��
    return (false);
}

