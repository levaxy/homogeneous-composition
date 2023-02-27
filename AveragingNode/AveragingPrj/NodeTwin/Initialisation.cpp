#include"Initialisation.h"

// Все функции вызываются из main

QJsonObject ReadJson(const QString& FileName){

    QString val;
    QFile Init;
    Init.setFileName(FileName + ".json");
    Init.open(QIODevice::ReadOnly | QIODevice::Text);
    val = Init.readAll();
    Init.close();

    QJsonParseError error;
    QJsonDocument JDoc  = QJsonDocument::fromJson(val.toUtf8(), &error);// не работает
    qDebug() << "Error: " << error.errorString() << error.offset << error.error;


    QJsonObject JObj;
    if (JDoc.isObject()){
        JObj = JDoc.object();
    }
    else
        cout<<"JDoc is not object\n";


    return JObj;
}

void InitOperations(const QJsonObject& AllData, vector<Operation*>& Operations, vector<Container*>& Containers){
    int tStart = AllData["TimeStart"].toInt();
    int tend = AllData["TimeEnd"].toInt();
    QJsonArray OpersArr = AllData["Operations"].toArray();
    for(auto i = OpersArr.begin(); i < OpersArr.end(); i++){

        size_t j = distance(OpersArr.begin(), i);
        QJsonObject oper = i->toObject();
        int IDcont = oper["IDCont"].toInt();
        int TimeInWork = oper["TimeInWork"].toInt();
        Operations[j]->SetRunTime(oper["RunTime"].toInt(), tStart, tend);
        for(Container*& c: Containers ){//ищем контейнер по ID
            if(c->ID == IDcont){
                Operations[j]->container = c;
            }
        }
        //если работает, записываем время окончания
        if(oper["condition"].toInt() == 1){
            Operations[j]->EndTime = tStart + Operations[j]->RunTime - TimeInWork;
        }

        Operations[j]->PPR = oper["PPR"].toInt();
        Operations[j]->Motoclock = oper["MotorTime"].toInt();
        Operations[j]->NewMotorTime = oper["FreshMotorTime"].toInt();
    }

}

vector<Cell> InitCells(const QJsonObject& AllData, vector<Container*>& Containers){
    vector<Cell> cells;
    QJsonArray Cells = AllData["Cells"].toArray();

    for(auto i = Cells.begin(); i < Cells.end(); i++){
        QJsonObject cell = i->toObject();
        Cell val;
        int IDCont = cell["IDCont"].toInt();
        if(IDCont == 0){//если нет контейнера
            val.container = &NullContainer;
            val.condition = 0;
        }
        else{//если есть, ищем по ID контейнера
            for (Container*& c: Containers) {
                if (c->ID == IDCont) {
                    val.container = c;
                    val.condition = 1;
                }
            }
        }

        val.ID = cell["ID"].toInt();
        cells.push_back(val);
    }
    return cells;
}
// Читает из Json объекта контейнеры.
vector<Container> InitContainers(const QJsonObject& AllData){
    vector<Container> containers;
    QJsonArray Containers = AllData["Containers"].toArray();
    for(auto i = Containers.begin(); i < Containers.end(); i++){
        QJsonObject container = i->toObject();
        Container val;
        val.ID = container["ID"].toInt();
        val.content = container["content"].toInt();
        containers.push_back(val);
    }
    return containers;
}

//
Manipulator InitManipulator(const QJsonObject& AllData, vector<Operation*>& Operations, vector<Cell*>& Cells){

    QJsonObject JManip = AllData["Manipulator"].toObject();
    Manipulator Manip;
    Manip.condition = JManip["condition"].toInt();
    int IDdestination = JManip["IDdestination"].toInt();//ID объекта, куда везёт
    Manip.Motoclock = JManip["MotorTime"].toInt();
    Manip.PPR = JManip["PPR"].toInt();
    Manip.NewMotorTime = JManip["FreshMotorTime"].toInt();//это полные моточасы

    if(Manip.condition == 1){
        if(IDdestination < 10 && IDdestination >= 0){	//проверяем что это это не гнездо и по ID ищем пункт назначения
            for(Operation*& o: Operations){
                if(o->ID == IDdestination){
                    Manip.Destination = o;
                }
            }
        }
        //если гнездо, то ищем среди гнёзд
        else if(IDdestination > 10){
            for(Cell*& cell : Cells){
                if(IDdestination == cell->ID){
                    Manip.Destination = cell;
                }
            }
        }

    }
    return Manip;
}


