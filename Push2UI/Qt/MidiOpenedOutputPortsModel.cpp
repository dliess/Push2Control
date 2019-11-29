#include "MidiOpenedOutputPortsModel.h"
#include "MidiRouter.h"

namespace push2::qt
{

MidiOpenedOutputPortsModel::MidiOpenedOutputPortsModel(MusicDeviceHolder& rMidiOpenedPorts,
                                                       midi::Router &rMidiRouter,
                                                       QObject *parent) :
    QAbstractListModel(parent),
    m_rMidiOpenedPorts(rMidiOpenedPorts),
    m_rMidiRouter(rMidiRouter)
{
    m_rMidiRouter.registerChangedCb([this](){
        update();
    });

}

QHash<int, QByteArray> MidiOpenedOutputPortsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PicturePath] = "picturepath";
    roles[SelectedRole] = "selected";
    roles[HasSpecializedChannelMapping] = "hasSpecializedChannelMapping";
    return roles;
}

int MidiOpenedOutputPortsModel::rowCount(const QModelIndex&) const
{
    return m_rMidiOpenedPorts.soundDevices.size();
}

QVariant MidiOpenedOutputPortsModel::data(const QModelIndex &index, int role) const
{
    const auto idx = index.row();
    switch(role)
    {
        case NameRole:
        {
            return getDevName(idx);
        }
        case PicturePath:
        {
            return getDevPicturePath(idx);
        }
        case SelectedRole:
        {
            return m_rMidiRouter.isRoutedTo(m_currentIndex, idx);
        }
        case HasSpecializedChannelMapping:
        {
            return m_rMidiRouter.hasSpecializedData(m_currentIndex, idx);
        }
        default:
        {
            break;
        }
    }
    return QVariant();
}

void MidiOpenedOutputPortsModel::update()
{
    emit dataChanged(createIndex(0,0),
                     createIndex(m_rMidiOpenedPorts.soundDevices.size() -1, 0),
                    { SelectedRole, HasSpecializedChannelMapping } );
}

int MidiOpenedOutputPortsModel::currentIndex() const
{
    return m_currentIndex;
}

void MidiOpenedOutputPortsModel::setCurrentIndex(int currentIndex)
{
    if(m_currentIndex != currentIndex)
    {
        m_currentIndex = currentIndex;
        update();
    }
}

void MidiOpenedOutputPortsModel::toggleRouted(int idx)
{
    m_rMidiRouter.toggleRouted(m_currentIndex, idx);
    update();
}

QString MidiOpenedOutputPortsModel::getDevName(int idx) const noexcept
{
    if(idx >=0 && idx < m_rMidiOpenedPorts.soundDevices.size())
    {
        return QString(m_rMidiOpenedPorts.soundDevices.atIdx(idx).second->pDescr->product_name.c_str());
    }
    return QString();
}

QString MidiOpenedOutputPortsModel::getDevPicturePath(int idx) const noexcept
{
    if(idx < m_rMidiOpenedPorts.soundDevices.size())
    {
        return QString(m_rMidiOpenedPorts.soundDevices.atIdx(idx).second->pDescr->thumbnail.c_str());
    }
    return QString();
}


} // namespace push2::qt