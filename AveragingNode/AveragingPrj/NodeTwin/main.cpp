﻿#include <iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<fstream>
#include <QDebug>

using namespace std;
//#include"IncObj.h"
//#include"IncludeJson.h"
#include"Initialisation.h"
#include"AddStatesOfStamp.h"

// В файл, эмитирующий БД записывает реальные состояния объектов к текущему моменту, но не меняет сами объекты
//Здесь не нужно ловить то что гнездо подаёт запрос, так как функция управления состояниями далее это сделает, но не на основе инфы из "БД", а через лабу. В Operation с этой целью метод проверки на конец выполнения CheckEnd, переопределён у хранилища, чтоб он всегда возвращал фолз.
void EmulatorBD(ofstream& BD, const vector<Operation*>& Objects, const Manipulator& manip, const int& current_time){
    BD<<current_time<<"\t";

   // manip.getStatus(current_time);
    BD << manip.getStatus(current_time) << "\t";

    for(const Operation* obj: Objects){
        int status = obj->getStatus(current_time);
        BD << status << "\t";
    }

    BD<<"\n";

}


// УПРАВЛЕНИЕ СОСТОЯНИЯМИ // Сравнивает с файлом эмитирующим БД состояния объектов, которые остались с прошлой итерации и если есть отличие, то вызывает Completer
bool Managing(ifstream& BD, vector<Operation*>& obj,Manipulator& manip){
    int condit = 0, time = 0, ConditManip = 0;
    bool change = false;
    BD>>time>>ConditManip;
    for(Operation*& o: obj){
        BD>>condit;
        if(condit != o->condition){
            if(o->condition == 3){
                o->CompletePPR();
                change = true;
            }
            else{
                o->Completer();
                change = true;
            }
        }
    }
    if(manip.condition != 0 && ConditManip == 0){
        manip.Completer();
        change = true;
    }
    else if(manip.condition == 3 && ConditManip == 0){
        manip.CompletePPR();
        change = true;
    }
    return change;
}

int main(){

    // Открытие файлов, запись подписей столбцов
    ///////////////////////////////////////////
    ofstream out;
    ofstream BDoutConditions;
    ofstream OutContainers;
    ofstream OutContainersID;
    ofstream OutMotoTime;
    ifstream BDin;
    out.open("Out_Condition.txt");
    out<<"time\tmanip\tgran\tpress\taverat\tlaba\tproba\tStZn\tcell_1\tcell_2\tcell_3\n";

    OutContainers.open("Out_Containers.txt");
    OutContainers << "time\tmanip\tgran\tpress\taverat\tlaba\tproba\tStZn\tcell_1\tcell_2\tcell_3\n";

    OutContainersID.open("Out_ContainersID.txt");
    OutContainersID << "time\tmanip\tgran\tpress\taverat\tlaba\tproba\tStZn\tcell_1\tcell_2\tcell_3\n";

    BDoutConditions.open("BD.txt");
    BDoutConditions << "time\tmanip\tgran\tpress\taverat\tlaba\tproba\tStZn\n";
    BDoutConditions.close();

    OutMotoTime.open("Out_MotoTime.txt");
    OutMotoTime << "time\tmanip\tgran\tpress\taverat\tlaba\tproba\tStZn\n";


    BDin.open("BD.txt");
    string s;
    getline(BDin,s);

    /////////////////////////////////////////////////////////////////////////////////////////

    // Объявление векторов указателей

    vector<Container*> PtrContainers;
    vector<Operation*> PtrObjects;
    vector<Cell*> PtrCells;
    vector<int> Queue;

    PtrCells.reserve(5);
    PtrContainers.reserve(5);
    Queue.reserve(5);
    PtrObjects.reserve(8);

   QJsonObject AllData = ReadJson("Init2");

    ////////////////////////////////////////////////////
    int tStart = AllData["TimeStart"].toInt(),
    tend = AllData["TimeEnd"].toInt();// время начала, конца моделирования
    // Массив контейнеров создаём
    vector<Container> Contnrs = InitContainers(AllData);
    for(Container& c: Contnrs){
        PtrContainers.push_back(&c);
    }

    ////////////////////////////////////////////////////////////
    vector<Cell> Cells = InitCells(AllData, PtrContainers);
    for(Cell& cell: Cells){
        PtrCells.push_back(&cell);
    }
    ////////////////////////////////////////////////////////////
    // Создаём объекты

    Storage stor;
    Granulating granulation;
    Averaging averaging;
    Probe probe;
    Laba lab;
    AddStZn addSt;
    Press press;

    // Налаживаем связи между ними
    granulation.NextOper = &averaging;
    averaging.NextOper = &probe;
    averaging.addStZn = & addSt;
    probe.laba = &lab;
    probe.store = &stor;
    probe.addStZn = &addSt;
    probe.NextOper = &stor;
    addSt.NextOper = &averaging;
    addSt.probe = &probe;
    addSt.aver = &averaging;
    press.aver = &averaging;
    press.NextOper = &granulation;
    //////////////////////////////////////////////

    // В каждое гнездо записываем операции, связанные с хранилищем
    for(Cell*& n: PtrCells){
        n->addstzn = &addSt;
        n->granulator = &granulation;
        n->press = &press;
    }
    stor.SetCells(PtrCells);// В поле хранилища записываем вектор с гнёздами

    // В вектор добавляем объекты
    PtrObjects.push_back(&granulation);//гран, пресс, уср, лаба, проба, стеарат
    PtrObjects.push_back(&press);
    PtrObjects.push_back(&averaging);
    PtrObjects.push_back(&lab);
    PtrObjects.push_back(&probe);
    PtrObjects.push_back(&addSt);

    ////////////////////////////////////////////////////////////
    InitOperations(AllData, PtrObjects, PtrContainers);
    averaging.SetParams();
    Manipulator manip = InitManipulator(AllData, PtrObjects, PtrCells);
    for(Container& c: Contnrs){
        if(c.content > 2){
            averaging.AverageRiskFunction(c);
        }
        if(c.content > 5){
            averaging.PowderingRiskFunction(c);
        }
    }
    ////////////////////////////////////////////////////////////

    //JSon document
    QJsonDocument jDoc;
    QJsonArray jMainArr;
    QJsonArray Batches;
    press.Batches = &Batches;
    if(manip.condition == 0)
        manip.ManipManaging(tStart, PtrObjects, Queue, &stor);
    for (size_t t = tStart; t < tend; t += 1){
        // Открытие файлов на запись текущей строчки(out and out_MotoTime каждую итерацию открываю и закрываю, чтоб при отладке можно было наблюдать, что туда пишется)
        ///////////////////////////////////////////////////////////
        out.open("Out_Condition.txt", ios::app);
        OutMotoTime.open("Out_MotoTime.txt",ios::app);
        BDoutConditions.open("BD.txt", ios::app);
        OutContainers.open("Out_Containers.txt", ios::app);
        OutContainersID.open("Out_ContainersID.txt", ios::app);


        // В BD пишутся состояния на текущий момент времени
        ////////////////////////////////////////////////////
        EmulatorBD(BDoutConditions, PtrObjects, manip, t);
        BDoutConditions.close();

        // Читает свежезаписанную строчку из BD, сравнивает с состояниями объектов и обновляет состояния объектов на основе этих данных
        // Managing возвращает bool. True если были изменения, false если без изменений
         // Если манипулятор не занят, то вызов манип. менеджера
        if(Managing(BDin, PtrObjects, manip) && manip.condition == 0){
            // если манип не занят, но и изменений не было(большая часть итераций как раз такая),
                                                      // то не будет дергаться ManipManaging
            manip.ManipManaging(t, PtrObjects, Queue, &stor);
        }
        /////////////////////////////////////////////////////
        Queue.clear();// очередь чистим

        // Выводим состояние манипулятора и его моточасы
        /////////////////////////////////////////////
        out<<t<<"\t";
        out<<manip.condition << "\t";

        OutMotoTime << t << "\t";
        OutMotoTime << manip.Motoclock << "\t";

        OutContainers << t << "\t";
        OutContainers << manip.container->content << "\t";

        OutContainersID << t << "\t";
        OutContainersID << manip.container->ID << "\t";

        // вывод состояний объектов
        /////////////////////////////////////////////
        for(Operation*& obj : PtrObjects){
            out<< obj->condition<<"\t";
            OutMotoTime<<obj->Motoclock<<"\t";
            OutContainers << obj->container->content << "\t";
            OutContainersID << obj->container->ID << "\t";
        }

        // Вывод содержимого контейнеров в гнёздах
        for (Cell*& Cell : stor.Cells) {
            out << Cell->container->content << "\t";
            OutContainers << Cell->container->content << "\t";
            OutContainersID << Cell->container->ID << "\t";
        }

        // Закрытие выходных файлов
        ////////////////////////////////////////
        out<<"\n";
        out.close();

        OutMotoTime<<"\n";
        OutMotoTime.close();

        OutContainers << "\n";
        OutContainers.close();

        OutContainersID << "\n";
        OutContainersID.close();
        /////////////////////////////////////////

        AddStatesToJMainArr(t, manip, PtrObjects, PtrCells, jMainArr);
    }
    jDoc.setArray(jMainArr);
    QFile f("statuses.js");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        f.write(jDoc.toJson());
        f.close();
    }
    QJsonDocument DocBatches;
    DocBatches.setArray(Batches);
    f.setFileName("Batches.js");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        f.write(DocBatches.toJson());
        f.close();
    }
    //Генерация файла с ресурсами из xml
//    QJsonDocument jResourcesDoc;
//    QJsonArray jArr;

//    QFile exp("experiments.txt");
//    bool b = exp.exists();

//    QList<QStringList> elements;
//    QStringList headers;

//    if (exp.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        QTextStream stream(&exp);
//        QString line;
//        while (stream.readLineInto(&line))
//        {
//            QStringList element = line.split("\t");
//            elements.append(element);
//        }
//    }

//    headers = elements.takeFirst();
//    headers.takeFirst(); // Выкидываем из заголовков "Порядковый номер"
//    headers.takeFirst(); // И выкидываем из заголовков момент производства "Порядковый номер"
//    QStringList coefs = elements.takeLast();
//    QJsonObject jMainObj;
//    QJsonArray dataArray;

//    jMainObj["headers"] = QJsonArray::fromStringList(headers);

//    foreach (auto e, elements)
//    {
//        QJsonObject jElement;
//        int idx = e.takeFirst().toInt();
//        jElement["index"] = idx;
//        jElement["fileName"] = QString("graph_%1.js").arg(idx);
//        jElement["creationMoment"] = e.takeFirst().replace(",",".").toDouble();
//        QJsonArray resources;

//        foreach (auto s, e)
//        {
//           resources.append(s.replace(",",".").toDouble());
//        }

//        jElement["resources"] = resources;
//        dataArray.append(jElement);
//    }

//    jMainObj["data"] = dataArray;

//    jResourcesDoc.setObject(jMainObj);

//    QFile resf("resources.js");
//    if ( resf.open(QIODevice::WriteOnly | QIODevice::Text) )
//    {
//        resf.write(jResourcesDoc.toJson());
//        resf.close();
//    }

//    //Генерация файлов с графиками
//    QDir dir(".");
//    QStringList fileNames = dir.entryList({"*.csv"}, QDir::Files);

//    foreach (auto fn, fileNames)
//    {
//        QFile gf(fn);
//        if (gf.open(QIODevice::ReadOnly | QIODevice::Text))
//        {
//            QTextStream stream(&gf);
//            QString line;
//            QList<QStringList> gElements;
//            while (stream.readLineInto(&line))
//            {
//                QStringList element = line.split(";");
//                gElements.append(element);
//            }


//            QStringList graphHeaders = gElements.takeFirst();

//            QJsonDocument graphDocument;
//            QJsonObject graphObject;

//            graphObject["headers"] = QJsonArray::fromStringList(graphHeaders);

//            QJsonArray graphArray;

//            foreach (auto e, gElements)
//            {
//                QJsonObject jElement;
//                jElement["x"] = e.takeFirst().toDouble();
//                jElement["y"] = e.takeFirst().toDouble();
//                graphArray.append(jElement);
//            }
//            graphObject["data"] = graphArray;

//            graphDocument.setObject(graphObject);

//            QString jFileName = fn.prepend("graph_").chopped(3).append("js");
//            QFile f(jFileName);
//            if ( f.open(QIODevice::WriteOnly | QIODevice::Text) )
//            {
//                f.write(graphDocument.toJson());
//                f.close();
//            }

//        }
//    }

    return 0;
}
//модиф прогу для работы в реал тайме(сделать делэями расчёт "в реал тайме", добавить функ. вылавливания нужных статусов для вызова каунтер тайм и модели усреднения)
//модифицировать как сказал АО (чётко разделить комментами каждое действие)
//9 - txt -> json
