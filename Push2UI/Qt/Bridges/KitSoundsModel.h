#ifndef KIT_SOUNDS_MODEL_H
#define KIT_SOUNDS_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "KitSoundVoicesModel.h"
#include "KitInstrument.h" // KitSound
#include "KitSoundBridge.h"

class KitSoundsModel : public QAbstractListModel
{
   Q_OBJECT
public:
   enum KitInstrumentsModelRole {
      NameRole = Qt::UserRole + 1,
      VoicesRole
   };

   KitSoundsModel(std::vector<KitSound>& rKitSounds) noexcept;
   QHash<int, QByteArray> roleNames() const override;
   int rowCount(const QModelIndex&) const override;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

   Q_INVOKABLE QAbstractListModel* voices(int soundIdx) noexcept;
private:
   std::vector<KitSoundBridge> m_kitSoundBridges;
};

#endif