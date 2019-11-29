#include "ControllerPresetSelectionModel.h"
#include "SelectedSoundDevice.h"

using namespace push2::qt;


ControllerPresetSelectionModel::ControllerPresetSelectionModel(MusicDeviceHolder&   rMidiOpenedPorts,
                                                               QObject *parent) :
    QAbstractListModel(parent),
    m_rMidiOpenedPorts(rMidiOpenedPorts)
{
}

QHash<int, QByteArray> ControllerPresetSelectionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

int ControllerPresetSelectionModel::rowCount(const QModelIndex&) const
{
    if(UNSELECTED == m_currentDeviceIndex) return 0;
    return m_rMidiOpenedPorts.controllerDevice(m_currentDeviceIndex)->pDescr->controllerSection->num_presets;
}

QVariant ControllerPresetSelectionModel::data(const QModelIndex &index, int role) const
{
    const auto idx = index.row();
    switch(role)
    {
        case NameRole:
        {
            return QString::number(idx);
        }
        default:
        {
            break;
        }
    }
    return QVariant();
}

int ControllerPresetSelectionModel::currentIndex() const
{
    return m_currentIndex;
}

void ControllerPresetSelectionModel::setCurrentIndex(int index)
{
    m_currentIndex = index;
}

int ControllerPresetSelectionModel::currentDeviceIndex() const
{
    return m_currentDeviceIndex;
}

void ControllerPresetSelectionModel::setCurrentDeviceIndex(int index)
{
    if(m_currentDeviceIndex != index)
    {
        QAbstractListModel::beginResetModel();
        m_currentDeviceIndex = index;
        QAbstractListModel::endResetModel();
    }
}