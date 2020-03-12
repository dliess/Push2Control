#ifndef KIT_INSTRUMENT_SOUND_VOICES_MODEL_H
#define KIT_INSTRUMENT_SOUND_VOICES_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "KitSoundVoicesModel.h"
#include "KitInstrument.h" // KitSound::Voices

class KitSoundVoicesModel : public QAbstractListModel
{
   Q_OBJECT
public:
   enum Role {
      DeviceNameRole = Qt::UserRole + 1,
      DevicePortRole,
      VoiceIndexRole,
      NoteOffsetRole
   };

   KitSoundVoicesModel(KitSound::Voices& rVoices) noexcept;

   QHash<int, QByteArray> roleNames() const override;
   int rowCount(const QModelIndex&) const override;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
   KitSound::Voices& m_rVoices;
};

#endif