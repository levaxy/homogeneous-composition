#include"Initialisation.h"

// Все функции вызываются из main

QJsonObject ReadJson(const QString& FileName){

    QByteArray val;
    QFile Init;
    Init.setFileName(FileName + ".json");
    Init.open(QIODevice::ReadOnly | QIODevice::Text);
    //val = Init.readAll();


// Считываем JSON из файла
    QByteArray jsonData = Init.readAll();
    Init.close();
    // Преобразуем JSON в объект QJsonObject
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        // Ошибка парсинга JSON
        return QJsonObject();
    }

    return jsonDoc.object();
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
        Operations[j]->condition = oper["condition"].toInt();
        if(Operations[j]->condition == 1){
            Operations[j]->EndTime = tStart + Operations[j]->RunTime - TimeInWork;
        }

        Operations[j]->PPR = oper["PPR"].toInt();
        Operations[j]->Motoclock = oper["MotorTime"].toInt();
        Operations[j]->NewMotorTime = oper["FreshMotorTime"].toInt();
        Operations[j]->CodePrior = oper["CodePrior"].toInt();
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
QList<Layer> convertJsonToLayerList(const QJsonArray& jsonArray) {
    QList<Layer> layerList;

    for (const QJsonValue& value : jsonArray) {
        const QJsonObject& object = value.toObject();

        Layer layer;
        layer.m_Pu = object["m_Pu"].toDouble();
        layer.Mass = object["Mass"].toDouble();
        layer.dencity = object["dencity"].toDouble();
        layer.V = object["V"].toDouble();
        layer.C_Pu = object["C_Pu"].toDouble();

        layerList.append(layer);
    }

    return layerList;
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
        val.Volume = container["Volume"].toDouble();
        if(val.content > 1){
            QJsonArray layers = container["Layers"].toArray();
            val.SetLayersBatch(convertJsonToLayerList(layers));

        }
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


