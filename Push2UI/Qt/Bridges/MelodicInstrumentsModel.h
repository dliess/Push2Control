#ifndef MELODIC_INSTRUMENTS_MODEL_H
#define MELODIC_INSTRUMENTS_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <vector>
#include "MelodicInstrumentBridge.h"

class MelodicInstrumentsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MelodicInstrumentsModelRole {
        NameRole = Qt::UserRole + 1,
        VoicesRole
    };

    MelodicInstrumentsModel(std::vector<MelodicInstrument>& rMelodicInstruments,
                            QObject* pParent) noexcept;

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE QAbstractListModel* voices(int melodicInstrIdx) noexcept;

private:
    std::vector<MelodicInstrumentBridge> m_melodicInstrumentBridges;
};

#endif