#ifndef PARAMETER_MODEL_H
#define PARAMETER_MODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QByteArray>
#include "BusinessLogic.h"

class ParameterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ParameterModelRole {
        NameRole = Qt::UserRole + 1,
        ValueRole
    };

    ParameterModel(std::vector<logic::Parameter>& rParameterVector, QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
private:
    std::vector<logic::Parameter>& m_rParameterVector;
};

#endif