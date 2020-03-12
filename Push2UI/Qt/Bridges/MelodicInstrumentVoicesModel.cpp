#include "MelodicInstrumentVoicesModel.h"

MelodicInstrumentVoicesModel::MelodicInstrumentVoicesModel(MelodicInstrument::VoiceContainer& rMelodicInstrumentVoices) noexcept:
   QAbstractListModel(),
   m_rVoices(rMelodicInstrumentVoices)
{}

QHash<int, QByteArray> MelodicInstrumentVoicesModel::roleNames() const
{
   QHash<int, QByteArray> roles;
   roles[DeviceNameRole] = "deviceName";
   roles[DevicePortRole] = "devicePort";
   roles[VoiceIndexRole] = "voiceIndex";
   return roles;
}

int MelodicInstrumentVoicesModel::rowCount(const QModelIndex&) const
{
   return m_rVoices.size();
}

QVariant MelodicInstrumentVoicesModel::data(const QModelIndex &index, int role) const
{
   if(!index.isValid()) return QVariant();
   switch(role)
   {
      case DeviceNameRole: return QString(m_rVoices[index.row()].soundDeviceId.deviceName.c_str());
      case DevicePortRole: return QString(m_rVoices[index.row()].soundDeviceId.portName.c_str());
      case VoiceIndexRole: return m_rVoices[index.row()].voiceIndex;
      default: break;
   }
   return QVariant();
}
