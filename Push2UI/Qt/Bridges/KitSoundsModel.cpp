#include "KitSoundsModel.h"

KitSoundsModel::KitSoundsModel(std::vector<KitSound>& rKitSounds) noexcept :
   QAbstractListModel()
{
   for(auto& kitSound : rKitSounds)
   {
      m_kitSoundBridges.emplace_back(kitSound);
   }
}

QHash<int, QByteArray> KitSoundsModel::roleNames() const
{
   QHash<int, QByteArray> roles;
   roles[NameRole] = "name";
   roles[VoicesRole] = "voices";
   return roles;
}

int KitSoundsModel::rowCount(const QModelIndex&) const
{
   return m_kitSoundBridges.size();
}

QVariant KitSoundsModel::data(const QModelIndex &index, int role) const
{
   if(!index.isValid()) return QVariant();
   switch(role)
   {
      case NameRole: return QString(m_kitSoundBridges[index.row()].rKitSound.name.c_str());
      case VoicesRole: return QVariant::fromValue(m_kitSoundBridges[index.row()].pKitSoundsModel.get());
      default: break;
   }
   return QVariant();
}

QAbstractListModel* KitSoundsModel::voices(int soundIdx) noexcept
{
   // TODO: assert sound index
   return m_kitSoundBridges[soundIdx].pKitSoundsModel.get();
}
