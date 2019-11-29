#ifndef CONTROLLER_PRESET_SELECTION_MODEL_H
#define CONTROLLER_PRESET_SELECTION_MODEL_H

#include "MusicDeviceHolder.h"
#include <QObject>
#include <QAbstractListModel>
#include <QHash>

namespace push2::qt
{

class ControllerPresetSelectionModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
    Q_PROPERTY(int currentDeviceIndex READ currentDeviceIndex WRITE setCurrentDeviceIndex)
public:
    enum BaseNoteRole {
        NameRole = Qt::UserRole + 1
    };

    ControllerPresetSelectionModel(MusicDeviceHolder&   rMidiOpenedPorts,
                                   QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int currentIndex() const;
    void setCurrentIndex(int index);

    int currentDeviceIndex() const;
    void setCurrentDeviceIndex(int index);

private:
    MusicDeviceHolder&   m_rMidiOpenedPorts;
    static constexpr int UNSELECTED = -1;
    int m_currentIndex{UNSELECTED};
    int m_currentDeviceIndex{UNSELECTED};
};

} // namespace push2::qt
#endif