#ifndef CARS_MODEL_H
#define CARS_MODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QByteArray>
#include <vector>

namespace logic { struct Cars; }
class ParameterModel;

class CarsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CarsModelRole {
        NameRole = Qt::UserRole + 1,
        ParameterRole
    };

    CarsModel(logic::Cars& cars, QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE QAbstractListModel* getParameterModel(int index) const noexcept;
private:
    logic::Cars& m_cars;
    std::vector<ParameterModel*> m_parameterModels;
};

#endif