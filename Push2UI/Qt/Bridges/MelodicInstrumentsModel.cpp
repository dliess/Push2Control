#include "MelodicInstrumentsModel.h"

MelodicInstrumentsModel::MelodicInstrumentsModel(std::vector<MelodicInstrument>& rMelodicInstruments) noexcept:
   QAbstractListModel()
{
   for(auto& melodicInstrument : rMelodicInstruments)
   {
      m_melodicInstrumentBridges.emplace_back(melodicInstrument);
   }
}

QHash<int, QByteArray> MelodicInstrumentsModel::roleNames() const
{
   QHash<int, QByteArray> roles;
   roles[NameRole] = "name";
   roles[VoicesRole] = "voices";
   return roles;
}

int MelodicInstrumentsModel::rowCount(const QModelIndex&) const
{
   return m_melodicInstrumentBridges.size();
}

QVariant MelodicInstrumentsModel::data(const QModelIndex &index, int role) const
{
   if(!index.isValid()) return QVariant();
   switch(role)
   {
      case NameRole: return  QString(m_melodicInstrumentBridges[index.row()].rMelodicInstrument.name().c_str());
      case VoicesRole: return QVariant::fromValue( m_melodicInstrumentBridges[index.row()].pMelodicInstrumentVoicesModel.get() );
      default: break;
   }
   return QVariant();
}

QAbstractListModel* MelodicInstrumentsModel::voices(int melodicInstrIdx) noexcept
{
   // TODO: check melodicInstrIdx
   m_melodicInstrumentBridges[melodicInstrIdx].pMelodicInstrumentVoicesModel.get();
}
