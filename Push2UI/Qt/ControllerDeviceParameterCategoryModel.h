#ifndef CONTROLLER_DEVICE_PARAMETER_CATEGORY_MODEL_H
#define CONTROLLER_DEVICE_PARAMETER_CATEGORY_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QHash>
#include "MusicDeviceHolder.h"

namespace push2::qt
{

class ControllerDeviceParameterCategoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MidiPortRoles {
        NameRole = Qt::UserRole + 1
    };

    ControllerDeviceParameterCategoryModel(MusicDeviceHolder& rMusicDeviceHolder,
                                           QObject*           parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void onCurrentDeviceIndexChanged(int index);

private:
    MusicDeviceHolder& m_rMusicDeviceHolder;
    static constexpr int UNSELECTED = -1;
    int m_currentDeviceIndex{UNSELECTED};
};

} // namespace push2::qt
#endif