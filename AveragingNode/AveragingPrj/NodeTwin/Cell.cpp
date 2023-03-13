#include"Cell.h"

//надо 
Cell:: Cell():Operation(){//полем кондитион не оперурием в этом классе, только содержимым контейнера, помещённого в гнездо
    this->addstzn = &NullOper;
    this->granulator = &NullOper;
    this->press = &NullOper;
    this->Motoclock = 1;// т.к. в конструкторе по умолчанию RunTime = 0, то моточасы не будут отбавляться у гнезда в комплитере
    this->CodePrior = 10;// при получении кода приоритетности в managing_manip если брак, то будет это число добавляться в очередь запросов.(Это код приоритетности увоза на переработку). Если придётся менять логику, что брак везут на пресс, то можно просто убрать весь код, связааный с увозом контейнера и привоза в то же место.
    this->Name = "Cell";
    N = 22/3;
}

Operation* Cell:: GetNextOper(){
    if(this->container->content == 1){//если пустой контейнер, то следующий пункт - это гранулятор
        return this->granulator;
    }
    if (this->container->content == 4) {//уср, не брак, то в стеар. цинка
        return addstzn;
    }
    if (this->container->content == 9 || this->container->content == 8 || this->container->content == 5) {//если готовый, либо брак со stz то на пресс
        return press;
    }
    return &NullOper;//в иных случаях следующей операции нет
}

bool Cell::CheckReady() {
    return (this->GetCondit() == 2 && this->GetNextOper()->GetCondit() == 0);
}

void Cell:: Beginner(const int& current_time, Container* cont){
    this->container = cont;
}

int Cell::GetCondit()const{
    if(this->container->content == 0){
        return 0;
    }
    else if(this->container->content == 7 || this->container->content == 3){
        return 1;
    }
    else if(this->container->content == 2 || this->container->content == 6){
        throw("Неусреднённый порошок попал в хранилище!\n");
    }
    else{
        return 2;
    }
}
