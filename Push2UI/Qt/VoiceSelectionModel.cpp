#include "VoiceSelectionModel.h"
#include "SelectedSoundDevice.h"

using namespace push2::qt;


VoiceSelectionModel::VoiceSelectionModel(MusicDeviceHolder&   rMidiOpenedPorts,
                                         QObject *parent) :
    QAbstractListModel(parent),
    m_rMidiOpenedPorts(rMidiOpenedPorts)
{
}

QHash<int, QByteArray> VoiceSelectionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

int VoiceSelectionModel::rowCount(const QModelIndex&) const
{
    if(UNSELECTED == m_currentDeviceIndex) return 0;
    return m_rMidiOpenedPorts.soundDevice(m_currentDeviceIndex)->pDescr->soundSection->voices.size();
}

QVariant VoiceSelectionModel::data(const QModelIndex &index, int role) const
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

int VoiceSelectionModel::currentIndex() const
{
    return m_currentIndex;
}

void VoiceSelectionModel::setCurrentIndex(int index)
{
    m_currentIndex = index;
}

int VoiceSelectionModel::currentDeviceIndex() const
{
    return m_currentDeviceIndex;
}

void VoiceSelectionModel::setCurrentDeviceIndex(int index)
{
    if(m_currentDeviceIndex != index)
    {
        QAbstractListModel::beginResetModel();
        m_currentDeviceIndex = index;
        QAbstractListModel::endResetModel();
    }
}