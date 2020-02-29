#include "InstrumentsBridge.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    InstrumentsBridge instrumentsBridge;
    QQmlApplicationEngine engine("qrc:/TestApp.qml");
    return app.exec();
}