#ifndef INSTRUMENTS_BRIDGE_H
#define INSTRUMENTS_BRIDGE_H

#include <QObject>
#include <memory>
#include <QQmlObjectListModel.h>
#include "KitInstrumentBridge.h"

class QAbstractItemModel;

class InstrumentsBridge
{
    Q_GADGET
public:
    InstrumentsBridge();
    Q_INVOKABLE QAbstractItemModel* kitInstruments() noexcept;
    Q_INVOKABLE QAbstractItemModel* melodicInstruments() noexcept;
private:
    std::unique_ptr<QQmlObjectListModel<KitInstrumentBridge>> m_kitInstruments;
    std::unique_ptr<QAbstractItemModel> m_melodicInstruments;
};

#endif