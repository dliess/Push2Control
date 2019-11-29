#ifndef MIDI_OPENED_INPUT_PORTS_MODEL_H
#define MIDI_OPENED_INPUT_PORTS_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QHash>
#include "MusicDeviceHolder.h"

namespace push2::qt
{

class MidiOpenedInputPortsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MidiPortRoles {
        NameRole = Qt::UserRole + 1,
        PicturePath /*,
        LastMidiMsg*/
    };

    MidiOpenedInputPortsModel(MusicDeviceHolder& rMidiOpenedPorts,
                              QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE QString getDevName(int idx) const noexcept;
    Q_INVOKABLE QUrl getDevPicturePath(int idx) const noexcept;
private:
    MusicDeviceHolder& m_rMidiOpenedPorts;
};

} // namespace push2::qt
#endif