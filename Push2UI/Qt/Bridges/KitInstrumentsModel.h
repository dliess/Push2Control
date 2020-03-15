#ifndef KIT_INSTRUMENTS_MODEL_H
#define KIT_INSTRUMENTS_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <vector>
#include "KitSoundsModel.h"
#include "KitInstrumentBridge.h"

class KitInstrument;

class KitInstrumentsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum KitInstrumentsModelRole {
        NameRole = Qt::UserRole + 1,
        SoundsRole
    };
    KitInstrumentsModel(std::vector<KitInstrument>& rKitInstruments, QObject* pParent) noexcept;
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE QAbstractListModel* sounds(int kitInstrIdx) noexcept;
private:
    std::vector<KitInstrumentBridge> m_kitInstrumentBridges;
};

#endif