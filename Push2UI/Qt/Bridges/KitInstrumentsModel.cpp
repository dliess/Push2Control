#include "KitInstrumentsModel.h"

KitInstrumentsModel::KitInstrumentsModel(std::vector<KitInstrument>& rKitInstruments) noexcept:
   QAbstractListModel()
{
   for(auto& kitInstrument : rKitInstruments)
   {
      m_kitInstrumentBridges.emplace_back(kitInstrument);
   }
}

QHash<int, QByteArray> KitInstrumentsModel::roleNames() const
{
   QHash<int, QByteArray> roles;
   roles[NameRole] = "name";
   roles[SoundsRole] = "sounds";
   return roles;
}

int KitInstrumentsModel::rowCount(const QModelIndex&) const
{
   return m_kitInstrumentBridges.size();
}

QVariant KitInstrumentsModel::data(const QModelIndex &index, int role) const
{
   if(!index.isValid()) return QVariant();
   switch(role)
   {
      case NameRole: return  QString(m_kitInstrumentBridges[index.row()].rKitInstrument.name().c_str());
      case SoundsRole: return QVariant::fromValue( m_kitInstrumentBridges[index.row()].pKitSoundsModel.get() );
      default: break;
   }
   return QVariant();
}

QAbstractListModel* KitInstrumentsModel::sounds(int kitInstrIdx) noexcept
{
   return m_kitInstrumentBridges[kitInstrIdx].pKitSoundsModel.get();
}