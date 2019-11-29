#ifndef CONTROLLER_DEVICE_SELECTION_MODEL_H
#define CONTROLLER_DEVICE_SELECTION_MODEL_H

#include "MusicDeviceHolder.h"
#include <QObject>
#include <QAbstractListModel>
#include <QHash>

namespace push2::qt
{

class ControllerDeviceSelectionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MidiPortRoles {
        NameRole = Qt::UserRole + 1,
        PicturePathRole
    };

    ControllerDeviceSelectionModel(MusicDeviceHolder&   rMidiOpenedPorts,
                              QObject*             parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    MusicDeviceHolder& m_rMidiOpenedPorts;
};

} // namespace push2::qt
#endif