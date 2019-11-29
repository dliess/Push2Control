#ifndef SOUND_DEVICE_SELECTION_MODEL_H
#define SOUND_DEVICE_SELECTION_MODEL_H

#include "MusicDeviceHolder.h"
#include <QObject>
#include <QAbstractListModel>
#include <QHash>

namespace push2::qt
{

class SoundDeviceSelectionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MidiPortRoles {
        NameRole = Qt::UserRole + 1,
        PicturePathRole
    };

    SoundDeviceSelectionModel(MusicDeviceHolder&   rMidiOpenedPorts,
                              QObject*             parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    MusicDeviceHolder& m_rMidiOpenedPorts;
};

} // namespace push2::qt
#endif