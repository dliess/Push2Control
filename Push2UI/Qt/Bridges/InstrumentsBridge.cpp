#include "InstrumentsBridge.h"
#include <QAbstractItemModel>

InstrumentsBridge::InstrumentsBridge()
{
    m_kitInstruments = std::make_unique<QQmlObjectListModel<KitInstrumentBridge>>();
    auto e = new KitInstrumentBridge;
    e->setProperty("intProperty", 3);
    e->setProperty("floatProperty", 3.14);
    e->setProperty("stringProperty", "Hello World");
    m_kitInstruments->append(e);
}

QAbstractItemModel* InstrumentsBridge::kitInstruments() noexcept
{
    return m_kitInstruments.get();
}

QAbstractItemModel* InstrumentsBridge::melodicInstruments() noexcept
{
    return m_melodicInstruments.get();
}
