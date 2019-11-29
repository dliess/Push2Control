#include "MidiDumperModel.h"

push2::qt::MidiDumperModel::MidiDumperModel(midi::Dumper& rMidiDumper) noexcept :
    m_rMidiDumper(rMidiDumper)
{
    m_rMidiDumper.registerRingbufChangedCb([this](int devIdx){
        if(devIdx == m_devIdx)
        {
            m_dataChanged = true;
        }
    });
    QObject::connect(&m_updateTimer, &QTimer::timeout, [this](){
        if(m_dataChanged)
        {
            m_dataChanged = false;
            QAbstractListModel::beginResetModel();
            m_data = m_rMidiDumper.getRingBuf(m_devIdx);
            QAbstractListModel::endResetModel();
            /*
            TODO: maybe optimize later, track if rowcount changes, and if not, use this: (instead of resetting the model all the time)
            emit dataChanged(createIndex(0, 0),
                             createIndex(rowCount(QModelIndex()) , 0),
                             { NameRole } );
            */
        }
        if(m_midiClockReceived != m_rMidiDumper.getClockReceived(m_devIdx))
        {
            m_midiClockReceived = m_rMidiDumper.getClockReceived(m_devIdx);
            emit midiClockReceivedChanged();
        } 
        m_rMidiDumper.resetClockReceived(m_devIdx);
    });
}

QHash<int, QByteArray> push2::qt::MidiDumperModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

int push2::qt::MidiDumperModel::rowCount(const QModelIndex&) const
{
    if(UNSET == m_devIdx) return 0;
    return m_data.Size();
}

QVariant push2::qt::MidiDumperModel::data(const QModelIndex &index, int role) const
{
    if(UNSET == m_devIdx) return QVariant();
    switch(role)
    {
        case NameRole:
        {
            const auto& msg = m_data[index.row()];
            QString ret;
            mpark::visit(midi::overload{
                [&ret](const mpark::monostate& invalid) {ret = "Invalid Midi-Msg"; },
                [&ret](auto&& all){ ret = all.toString().c_str(); }
            }, msg);
            return ret;
        }
        default: break;
    }
    return QVariant();
}

void push2::qt::MidiDumperModel::setMidiInputDeviceIdx(int idx) noexcept
{
    if(m_devIdx == idx) return;
    m_updateTimer.stop();
    QAbstractListModel::beginResetModel();
    m_devIdx = idx;
    m_data = m_rMidiDumper.getRingBuf(idx);
    QAbstractListModel::endResetModel();
    m_updateTimer.start(UI_UPDATE_CYCLE_MS);
}

bool push2::qt::MidiDumperModel::midiClockReceived() const noexcept
{
    return m_midiClockReceived;
}

