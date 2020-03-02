#include "InstrumentsBridge.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    InstrumentsBridge instrumentsBridge;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("instrumentsBridge", &instrumentsBridge);
    qRegisterMetaType<QAbstractListModel*>("QAbstractListModel*");
    engine.load("qrc:/TestApp.qml");
    return app.exec();
}