#ifndef KIT_INSTRUMENTS_BRIDGE_H
#define KIT_INSTRUMENTS_BRIDGE_H

#include <QObject>
#include <QString>

class KitInstrumentBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int intProperty MEMBER m_intProperty NOTIFY intPropertyChanged)
    Q_PROPERTY(float floatProperty MEMBER m_floatProperty NOTIFY floatPropertyChanged)
    Q_PROPERTY(QString stringProperty MEMBER m_stringProperty NOTIFY stringPropertyChanged)
signals:
    void intPropertyChanged();
    void floatPropertyChanged();
    void stringPropertyChanged();
private:
    int     m_intProperty;
    float   m_floatProperty;
    QString m_stringProperty;
};

#endif