#include <iostream>
#include<vector>
#include<algorithm>
#include<string>
using namespace std;
#include"Operation.h"
#include"Granulating.h"
#include"Averaging.h"
#include"AddStZn.h"
#include"Granulating.h"
#include"Manipulator.h"
#include"Container.h"
#include"Laba.h"
#include"Cell.h"
#include"Press.h"
#include"Storage.h"
#include"Probe.h"
#include<fstream>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>

// Предустановка // Из стартового файла берёт начальное состояние системы

void Init(ifstream& in, vector<Operation*>& obj, Manipulator& manip, Storage& stor, int& tStart, int& tend, vector<Container*>& conteiners, vector<Cell*>& Cells){
    string s;
    in>>tStart>>tend;
    for(Container* c: conteiners){
        in>>c->content>>c->ID;
    }
    //читаем данные контейнеров
    int TimeInWork = 0, IDcont = 0, PPR = 0, MotorTime = 0, NewMotorTime = 0, RunTime = 0;
    //читаем даные об объектах
    for(Operation*& o: obj){
        in>>o->condition>>TimeInWork>>o->CodePrior>>IDcont>>PPR>>MotorTime>>NewMotorTime>>RunTime;
        //считали данные из строки
        o->SetRunTime(RunTime, tStart, tend);//заполняем массив случайными числами (RunTime + (0-10)%)
        for(Container*& c: conteiners ){//ищем контейнер по ID
            if(c->ID == IDcont){
                o->container = c;
            }
        }
        //если работает, записываем время окончания
        if(o->condition == 1){
            o->EndTime = tStart + o->RunTime - TimeInWork;
        }

        o->PPR = PPR;
        o->Motoclock = MotorTime;
        o->NewMotorTime = NewMotorTime;

    }
    //читаем данные о гнездах хранилища
    for(Cell*& n: Cells){
        in>>IDcont>>n->ID;
        if(IDcont == 0){//если нет контейнера
            n->container = &NullContainer;
            n->condition = 0;
        }
        else{//если есть, ищем по ID контейнера
            for (Container*& c : conteiners) {
                if (c->ID == IDcont) {
                    n->container = c;
                    n->condition = 1;
                }
            }
        }
    }
    //читаем манипулятор
    int IDdestination = 0;//ID объекта, куда везёт
    in>>manip.condition>>IDcont>> IDdestination >> manip.TimeEnd >> PPR >> MotorTime >> NewMotorTime;


    manip.Motoclock = MotorTime;
    manip.PPR = PPR;
    manip.NewMotorTime = NewMotorTime;//это полные моточасы


    if(manip.condition == 1){
        if(IDdestination<10 && IDdestination >= 0){	//проверяем что это это не гнездо и по ID ищем пункт назначения
            for(Operation*& o: obj){
                if(o->ID == IDdestination){
                    manip.Destination = o;
                }
            }
        }
        //если гнездо, то ищем среди гнёзд
        else if(IDdestination>10){
            for(Cell*& n : Cells){
                if(IDdestination == n->ID){
                    manip.Destination = n;
                }
            }
        }

    }

}


// В файл, эмитирующий БД записывает реальные состояния объектов к текущему моменту, но не меняет сами объекты
//Здесь не нужно ловить то что гнездо подаёт запрос, так как функция управления состояниями далее это сделает, но не на основе инфы из "БД", а через лабу. В Operation с этой целью метод проверки на конец выполнения CheckEnd, переопределён у хранилища, чтоб он всегда возвращал фолз.
void EmulatorBD(ofstream& BD, const vector<Operation*>& Objects, const Manipulator& manip, const int& current_time){
    BD<<current_time<<"\t";

    manip.getStatus(current_time);
    BD << manip.getStatus(current_time) << "\t";

    for(const Operation* obj: Objects){
        int status = obj->getStatus(current_time);
        BD << status << "\t";
    }

    BD<<"\n";

}


// УПРАВЛЕНИЕ СОСТОЯНИЯМИ // Сравнивает с файлом эмитирующим БД состояния объектов, которые остались с прошлой итерации и если есть отличие, то вызывает Completer
void Managing(ifstream& BD, vector<Operation*>& obj,Manipulator& manip){
    int condit = 0, time = 0, ConditManip = 0;
    BD>>time>>ConditManip;
    for(Operation*& o: obj){
        BD>>condit;
        if(condit != o->condition){
            if(o->condition == 3){
                o->CompletePPR();
            }
            else{
                o->Completer();
            }
        }
    }
    if(manip.container->ID != 0 && ConditManip == 0){
        manip.Completer();
    }
    else if(manip.condition == 3 && ConditManip == 0){
        manip.CompletePPR();
    }
}

int main(){

    // Открытие файлов, запись подписей столбцов
    ///////////////////////////////////////////

    ifstream init;
    init.open("Init1.txt");
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


    // Объявление векторов

    vector<Container*> containers;
    vector<Operation*> Objects;
    vector<Cell*> Cells;
    vector<int> Queue;

    int tStart = 0, tend = 0, QuantCont = 4;// время начала, конца моделирования, количество контейнеров на линии
    // Говорим вектору, сколько там будет элементов, чтоб при добавлении очередного элемента не перевыделялась память заново
    Cells.reserve(3);
    containers.reserve(QuantCont);
    Queue.reserve(QuantCont);
    Objects.reserve(8);

    // Массив контейнеров создаём
    Container c1{};
    Container c2{};
    Container c3{};
    Container c4{};

    containers.push_back(&c1);
    containers.push_back(&c2);
    containers.push_back(&c3);
    containers.push_back(&c4);
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    // Создаём объекты
    Manipulator manip;
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
    probe.laba = &lab;
    probe.NextOper = &stor;
    addSt.NextOper = &averaging;
    press.aver = &averaging;
    press.NextOper = &granulation;
    //////////////////////////////////////////////
    // Создание массива гнёзд
    Cell Cell1{};
    Cell Cell2{};
    Cell Cell3{};



    Cells.push_back(&Cell1);
    Cells.push_back(&Cell2);
    Cells.push_back(&Cell3);

    // Каждое гнездо знакомим с операциями, связанными с хранилищем
    for(Cell*& n: Cells){
        n->addstzn = &addSt;
        n->granulator = &granulation;
        n->press = &press;
    }
    stor.SetCells(Cells);// В поле хранилища записываем вектор с гнёздами

    // В вектор добавляем объекты
    Objects.push_back(&granulation);//гран, пресс, уср, лаба, проба, стеарат
    Objects.push_back(&press);
    Objects.push_back(&averaging);
    Objects.push_back(&lab);
    Objects.push_back(&probe);
    Objects.push_back(&addSt);

    ////////////////////////////////////////////////////////////

    Init(init, Objects, manip, stor, tStart, tend, containers, Cells);

    ////////////////////////////////////////////////////////////

    //JSon document
    QJsonDocument jDoc;
    QJsonArray jMainArr;

    for (size_t t = tStart; t < tend; t+=10){
        // Открытие файлов на запись текущей строчки(out and out_MotoTime каждую итерацию открываю и закрываю, чтоб при отладке можно было наблюдать, что туда пишется)
        ///////////////////////////////////////////////////////////
        out.open("Out_Condition.txt", ios::app);
        OutMotoTime.open("Out_MotoTime.txt",ios::app);
        BDoutConditions.open("BD.txt", ios::app);
        OutContainers.open("Out_Containers.txt", ios::app);
        OutContainersID.open("Out_ContainersID.txt", ios::app);


        // В BD пишутся состояния на текущий момент времени
        ////////////////////////////////////////////////////
        EmulatorBD(BDoutConditions, Objects, manip, t);
        BDoutConditions.close();

        Managing(BDin, Objects, manip);// Читает свежезаписанную строчку из BD, сравнивает с состояниями объектов и обновляет состояния объектов на основе этих данных

        // Если манипулятор не занят, то вызов манип. менеджера
        if(manip.condition == 0){
            manip.ManipManaging(t, Objects, Queue, &stor);
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
        for(Operation*& obj : Objects){
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

        foreach (auto o, Objects)
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

        foreach (auto c, Cells)
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
    jDoc.setArray(jMainArr);
    QFile f("statuses.js");
    if (f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        f.write(jDoc.toJson());
        f.close();
    }


    //Генерация файла с ресурсами из xml
    QJsonDocument jResourcesDoc;
    QJsonArray jArr;

    QFile exp("experiments.txt");
    bool b = exp.exists();

    QList<QStringList> elements;
    QStringList headers;

    if (exp.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&exp);
        QString line;
        while (stream.readLineInto(&line))
        {
            QStringList element = line.split("\t");
            elements.append(element);
        }
    }

    headers = elements.takeFirst();
    headers.takeFirst(); // Выкидываем из заголовков "Порядковый номер"
    headers.takeFirst(); // И выкидываем из заголовков момент производства "Порядковый номер"
    QStringList coefs = elements.takeLast();
    QJsonObject jMainObj;
    QJsonArray dataArray;

    jMainObj["headers"] = QJsonArray::fromStringList(headers);

    foreach (auto e, elements)
    {
        QJsonObject jElement;
        int idx = e.takeFirst().toInt();
        jElement["index"] = idx;
        jElement["fileName"] = QString("graph_%1.js").arg(idx);
        jElement["creationMoment"] = e.takeFirst().replace(",",".").toDouble();
        QJsonArray resources;

        foreach (auto s, e)
        {
           resources.append(s.replace(",",".").toDouble());
        }

        jElement["resources"] = resources;
        dataArray.append(jElement);
    }

    jMainObj["data"] = dataArray;

    jResourcesDoc.setObject(jMainObj);

    QFile resf("resources.js");
    if ( resf.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        resf.write(jResourcesDoc.toJson());
        resf.close();
    }

    //Генерация файлов с графиками
    QDir dir(".");
    QStringList fileNames = dir.entryList({"*.csv"}, QDir::Files);

    foreach (auto fn, fileNames)
    {
        QFile gf(fn);
        if (gf.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&gf);
            QString line;
            QList<QStringList> gElements;
            while (stream.readLineInto(&line))
            {
                QStringList element = line.split(";");
                gElements.append(element);
            }


            QStringList graphHeaders = gElements.takeFirst();

            QJsonDocument graphDocument;
            QJsonObject graphObject;

            graphObject["headers"] = QJsonArray::fromStringList(graphHeaders);

            QJsonArray graphArray;

            foreach (auto e, gElements)
            {
                QJsonObject jElement;
                jElement["x"] = e.takeFirst().toDouble();
                jElement["y"] = e.takeFirst().toDouble();
                graphArray.append(jElement);
            }
            graphObject["data"] = graphArray;

            graphDocument.setObject(graphObject);

            QString jFileName = fn.prepend("graph_").chopped(3).append("js");
            QFile f(jFileName);
            if ( f.open(QIODevice::WriteOnly | QIODevice::Text) )
            {
                f.write(graphDocument.toJson());
                f.close();
            }

        }
    }

    return 0;
}
//модиф прогу для работы в реал тайме(сделать делэями расчёт "в реал тайме", добавить функ. вылавливания нужных статусов для вызова каунтер тайм и модели усреднения)
//модифицировать как сказал АО (чётко разделить комментами каждое действие)
//9 - txt -> json
