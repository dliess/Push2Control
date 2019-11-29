#ifndef MIDI_DUMP_MODEL_H
#define MIDI_DUMP_MODEL_H

#include <QAbstractListModel>
#include <QTimer>
#include <atomic>
#include "MidiDumper.h"


namespace push2::qt
{

class MidiDumperModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool midiClockReceived READ midiClockReceived NOTIFY midiClockReceivedChanged)
public:
    MidiDumperModel(midi::Dumper& rMidiDumper) noexcept;
    enum Roles {
        NameRole = Qt::UserRole + 1
    };
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void setMidiInputDeviceIdx(int idx) noexcept;
    bool midiClockReceived() const noexcept;

private:
    midi::Dumper&     m_rMidiDumper;
    midi::Dumper::RingBuf m_data;
    std::atomic<bool> m_dataChanged{false};
    bool              m_midiClockReceived{false};
    static constexpr int UNSET = -1;
    static constexpr int UI_UPDATE_CYCLE_MS = 200;
    int               m_devIdx{UNSET};
    QTimer            m_updateTimer;

signals:
    void midiClockReceivedChanged();
};

} // namespace push2::qt

#endif