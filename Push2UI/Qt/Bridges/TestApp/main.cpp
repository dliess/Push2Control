#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "BusinessLogic.h"
#include "CarsModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    logic::Cars cars;
    cars.data.push_back( logic::Car{"BlueCar", {{"Color", 1}, {"WheelCount", 4}, {"Height", 129}} } );
    cars.data.push_back( logic::Car{"RedCar",  {{"Endurance", 30}, {"NumDoors", 4}} } );
    cars.data.push_back( logic::Car{"GreenCar",  {{"Endurance", 32}, {"NumDoors", 10}} } );
    CarsModel carsModel(cars);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("cars", &carsModel);
    qRegisterMetaType<QAbstractListModel*>("QAbstractListModel*");
    engine.load("qrc:/TestApp.qml");
    return app.exec();
}
