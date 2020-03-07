#include "ParameterModel.h"
#include "BusinessLogic.h"

ParameterModel::ParameterModel(std::vector<logic::Parameter>& rParameterVector, QObject *parent) :
    QAbstractListModel(),
    m_rParameterVector(rParameterVector)
{}

QHash<int, QByteArray> ParameterModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ValueRole] = "value";
    return roles;
}

int ParameterModel::rowCount(const QModelIndex&) const
{
    m_rParameterVector.size();
}

QVariant ParameterModel::data(const QModelIndex &index, int role) const
{
    switch(role)
    {
        case NameRole: return QString(m_rParameterVector[index.row()].name.c_str());
        case ValueRole: return m_rParameterVector[index.row()].value;
        default: return QVariant();
    }
}
