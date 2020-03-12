#include "InstrumentsBridge.h"
#include <QAbstractItemModel>
#include "Instruments.h"

InstrumentsBridge::InstrumentsBridge(Instruments& rInstruments) noexcept:
    QObject(),
    m_kitInstrumentsModel(rInstruments.kitInstruments),
    m_melodicInstrumentsModel(rInstruments.melodicInstruments)
{
}

QAbstractListModel* InstrumentsBridge::kitInstruments() noexcept
{
    return &m_kitInstrumentsModel;
}

QAbstractListModel* InstrumentsBridge::melodicInstruments() noexcept
{
    return &m_melodicInstrumentsModel;
}
