#include "CarsModel.h"
#include "BusinessLogic.h"
#include "ParameterModel.h"

CarsModel::CarsModel(logic::Cars& cars, QObject *parent ) :
    QAbstractListModel(),
    m_cars(cars)
{
    for(int i = 0; i < cars.data.size(); ++i)
    {
        m_parameterModels.push_back(new ParameterModel(cars.data[i].parameter, this));
    }
}

QHash<int, QByteArray> CarsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ParameterRole] = "parameter";
    return roles;
}

int CarsModel::rowCount(const QModelIndex&) const
{
    m_cars.data.size();
}

QVariant CarsModel::data(const QModelIndex &index, int role) const
{
    switch(role)
    {
        case NameRole: return QString(m_cars.data[index.row()].name.c_str());
        case ParameterRole: return QVariant::fromValue(m_parameterModels[index.row()]);
        default: return QVariant();
    }
}

QAbstractListModel* CarsModel::getParameterModel(int index) const noexcept
{
    if(index < 0 || index >= m_parameterModels.size())
    {
        return Q_NULLPTR;
    }
    return m_parameterModels[index];
}

