#ifndef INSTRUMENTS_BRIDGE_H
#define INSTRUMENTS_BRIDGE_H

#include <QObject>
#include <memory>
#include <QAbstractListModel>
#include "KitInstrumentsModel.h"
#include "MelodicInstrumentsModel.h"

class Instruments;

class InstrumentsBridge : public QObject
{
    Q_OBJECT
public:
    InstrumentsBridge(Instruments& rInstruments) noexcept;
    Q_INVOKABLE QAbstractListModel* kitInstruments() noexcept;
    Q_INVOKABLE QAbstractListModel* melodicInstruments() noexcept;
private:
    KitInstrumentsModel*     m_pKitInstrumentsModel;
    MelodicInstrumentsModel* m_pMelodicInstrumentsModel;
};

#endif