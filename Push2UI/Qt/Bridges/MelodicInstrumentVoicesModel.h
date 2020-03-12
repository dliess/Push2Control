#ifndef MELODIC_INSTRUMENT_VOICE_MODEL_H
#define MELODIC_INSTRUMENT_VOICE_MODEL_H

#include <QAbstractListModel>
#include "MelodicInstrument.h"

class MelodicInstrumentVoicesModel : public QAbstractListModel
{
   Q_OBJECT
public:
   enum Role {
      DeviceNameRole = Qt::UserRole + 1,
      DevicePortRole,
      VoiceIndexRole
   };

   MelodicInstrumentVoicesModel(MelodicInstrument::VoiceContainer& rMelodicInstrumentVoices) noexcept;

   QHash<int, QByteArray> roleNames() const override;
   int rowCount(const QModelIndex&) const override;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
   MelodicInstrument::VoiceContainer& m_rVoices;
};

#endif