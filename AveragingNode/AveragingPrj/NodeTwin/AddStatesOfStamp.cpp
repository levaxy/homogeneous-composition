#include"AddStatesOfStamp.h"
void AddStatesToJMainArr(const int& t, const Manipulator& manip, const vector<Operation*>& PtrObjects,
                         const vector<Cell*>& PtrCells, QJsonArray& jMainArr){
    //export data to json.
    QJsonArray dataArr;
    QJsonObject timeStamp;
    timeStamp["time"] = QString::number(t);

    QJsonObject jManip;

    jManip["plantID"] = manip.ID;
    jManip["plantName"] = manip.Name;
    jManip["state"] = manip.getStatus(t);
    jManip["moto"] = QJsonArray{manip.Motoclock};
    jManip["containerID"] = manip.container->ID;
    jManip["containerContent"] = manip.container->content;

    QJsonArray jManipRes;
    QJsonObject jManipPwr;

    jManipPwr["sideheader"] = "Потребляемая мощность, кВт";
    jManipPwr["values"] = QJsonArray{manip.getPower()};
    jManipRes.append(jManipPwr);

    jManip["res"] = jManipRes;

    dataArr.append(jManip);

    for (Operation* o: PtrObjects)
    {
        QJsonObject plant;

        plant["plantID"] = o->ID;
        plant["plantName"] = o->Name;
        plant["state"] = o->getStatus(t);
        plant["moto"] = QJsonArray{o->Motoclock};
        plant["containerID"] = o->container->ID;
        plant["containerContent"] = o->container->content;

        QJsonArray jResArr;
        QJsonObject jResPwr;
        QJsonObject jResN;

        jResPwr["sideheader"] = "Потребляемая мощность, кВт";
        jResPwr["values"] = QJsonArray{o->getPower()};
        jResN["sideheader"] = "Расход азота, м3/ч";
        jResN["values"] = QJsonArray{o->getN()};
        jResArr.append(jResPwr);
        jResArr.append(jResN);

        plant["res"] = jResArr;

        dataArr.append(plant);
    }

    for (Cell* c: PtrCells)
    {
        QJsonObject jCell;

        jCell["plantID"] = c->ID;
        jCell["plantName"] = c->Name;
        jCell["state"] = c->getStatus(t);
        jCell["moto"] = QJsonArray{c->Motoclock};
        jCell["containerID"] = c->container->ID;
        jCell["containerContent"] = c->container->content;

        QJsonArray jResArr;
        QJsonObject jResN;

        jResN["sideheader"] = "Расход азота, м3/ч";
        jResN["values"] = QJsonArray{c->getN()};
        jResArr.append(jResN);

        jCell["res"] = jResArr;

        dataArr.append(jCell);
    }

    timeStamp["data"] = dataArr;

    jMainArr.append(timeStamp);
}
