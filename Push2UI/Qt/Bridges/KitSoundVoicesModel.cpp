#include "KitSoundsModel.h"

KitSoundVoicesModel::KitSoundVoicesModel(KitSound::Voices& rVoices) noexcept:
   QAbstractListModel(),
   m_rVoices(rVoices)
{}


QHash<int, QByteArray> KitSoundVoicesModel::roleNames() const
{
   QHash<int, QByteArray> roles;
   roles[DeviceNameRole] = "deviceName";
   roles[DevicePortRole] = "devicePort";
   roles[VoiceIndexRole] = "voiceIndex";
   roles[NoteOffsetRole] = "noteOffset";
   return roles;
}

int KitSoundVoicesModel::rowCount(const QModelIndex&) const
{
   return m_rVoices.size();
}

QVariant KitSoundVoicesModel::data(const QModelIndex &index, int role) const
{
   if(!index.isValid()) return QVariant();
   switch(role)
   {
      case DeviceNameRole: return QString(m_rVoices[index.row()].soundDeviceId.deviceName.c_str());
      case DevicePortRole: return QString(m_rVoices[index.row()].soundDeviceId.portName.c_str());
      case VoiceIndexRole: return m_rVoices[index.row()].voiceIndex;
      case NoteOffsetRole: return m_rVoices[index.row()].noteOffset;
      default: break;
   }
   return QVariant();
}
