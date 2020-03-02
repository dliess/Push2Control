#ifndef INSTRUMENTS_BRIDGE_H
#define INSTRUMENTS_BRIDGE_H

#include <QObject>
#include <memory>
#include <QQmlObjectListModel.h>
#include "KitInstrumentBridge.h"

#include <QAbstractListModel>
//class QAbstractListModel;

class InstrumentsBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* kitInstruments READ kitInstruments CONSTANT)
public:
    InstrumentsBridge();
    QAbstractListModel* kitInstruments() const noexcept;
    Q_INVOKABLE QAbstractListModel* melodicInstruments() noexcept;
private:
    std::unique_ptr<QQmlObjectListModel<KitInstrumentBridge>> m_kitInstruments;
    std::unique_ptr<QQmlObjectListModel<KitInstrumentBridge>> m_melodicInstruments;
};

#endif