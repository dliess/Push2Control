#include "InstrumentsBridge.h"
#include <QAbstractItemModel>

InstrumentsBridge::InstrumentsBridge()
{
    m_kitInstruments = std::make_unique<QQmlObjectListModel<KitInstrumentBridge>>();
    {
        auto e = new KitInstrumentBridge;
        e->setProperty("intProperty", 3);
        e->setProperty("floatProperty", 3.14);
        e->setProperty("stringProperty", "Hello World");
        m_kitInstruments->append(e);
    }
    {
        auto e = new KitInstrumentBridge;
        e->setProperty("intProperty", 3);
        e->setProperty("floatProperty", 3.14);
        e->setProperty("stringProperty", "Hello World");
        m_kitInstruments->append(e);
    }
    {
        auto e = new KitInstrumentBridge;
        e->setProperty("intProperty", 3);
        e->setProperty("floatProperty", 3.14);
        e->setProperty("stringProperty", "Hello World");
        m_kitInstruments->append(e);
    }
    {
        auto e = new KitInstrumentBridge;
        e->setProperty("intProperty", 3);
        e->setProperty("floatProperty", 3.14);
        e->setProperty("stringProperty", "Hello World");
        m_kitInstruments->append(e);
    }
    {
        auto e = new KitInstrumentBridge;
        e->setProperty("intProperty", 3);
        e->setProperty("floatProperty", 3.14);
        e->setProperty("stringProperty", "Hello World");
        m_kitInstruments->append(e);
    }

    m_melodicInstruments = std::make_unique<QQmlObjectListModel<KitInstrumentBridge>>();
    {
        auto e = new KitInstrumentBridge;
        e->setProperty("intProperty", 1);
        e->setProperty("floatProperty", 3.14);
        e->setProperty("stringProperty", "Hello World");
        m_melodicInstruments->append(e);
    }
    {
        auto e = new KitInstrumentBridge;
        e->setProperty("intProperty", 3);
        e->setProperty("floatProperty", 3.14);
        e->setProperty("stringProperty", "Hello World");
        m_melodicInstruments->append(e);
    }
    {
        auto e = new KitInstrumentBridge;
        e->setProperty("intProperty", 5);
        e->setProperty("floatProperty", 3.14);
        e->setProperty("stringProperty", "Hello World");
        m_melodicInstruments->append(e);
    }
}

QAbstractListModel* InstrumentsBridge::kitInstruments() const noexcept
{
    return m_kitInstruments.get();
}

QAbstractListModel* InstrumentsBridge::melodicInstruments() noexcept
{
    return m_melodicInstruments.get();
}
