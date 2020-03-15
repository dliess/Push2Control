#include "InstrumentsBridge.h"
#include <QAbstractItemModel>
#include "Instruments.h"

InstrumentsBridge::InstrumentsBridge(Instruments& rInstruments) noexcept:
    QObject(),
    m_pKitInstrumentsModel(new KitInstrumentsModel(rInstruments.kitInstruments, this)),
    m_pMelodicInstrumentsModel(new MelodicInstrumentsModel(rInstruments.melodicInstruments, this))
{
}

QAbstractListModel* InstrumentsBridge::kitInstruments() noexcept
{
    return m_pKitInstrumentsModel;
}

QAbstractListModel* InstrumentsBridge::melodicInstruments() noexcept
{
    return m_pMelodicInstrumentsModel;
}
