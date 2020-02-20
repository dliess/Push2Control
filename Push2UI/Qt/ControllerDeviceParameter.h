#ifndef CONTROLLER_DEVICE_PARAMETER_H
#define CONTROLLER_DEVICE_PARAMETER_H

#include <QObject>
#include "ControllerDeviceParameterCategoryModel.h"
#include "ControllerDeviceParameterModel.h"

struct MusicDeviceHolder;
class ParameterMapper;

namespace push2::qt
{

class ControllerDeviceParameter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* parameterCategoryModel READ parameterCategoryModel NOTIFY parameterCategoryModelChanged)
    Q_PROPERTY(QAbstractListModel* parameterModel READ parameterModel NOTIFY parameterModelChanged)
    Q_PROPERTY(int currentCategoryIndex READ currentCategoryIndex)
    Q_PROPERTY(int currentParameterInCategoryIndex READ currentParameterInCategoryIndex)

public:
    explicit ControllerDeviceParameter(MusicDeviceHolder& rMusicDeviceHolder,
                                       ParameterMapper&   rParameterMapper);
    QAbstractListModel* parameterCategoryModel();
    QAbstractListModel* parameterModel();

    int currentCategoryIndex() const;
    Q_INVOKABLE void setCurrentCategoryIndex(int index);

    int currentParameterInCategoryIndex() const;
    Q_INVOKABLE void setCurrentParameterInCategoryIndex(int index);

    Q_INVOKABLE void incrementValueMapOutAtMaxAmount(int parameterInCategoryIndex, float increment);
    Q_INVOKABLE void incrementValueMapOutAtMinAmount(int parameterInCategoryIndex, float increment);
    Q_INVOKABLE void incrementValueMapBendAmount(int parameterInCategoryIndex, qreal increment);

public slots:
    void onCurrentDeviceIndexChanged(int index);
    void onCurrentPresetIndexChanged(int index);

private:
    ControllerDeviceParameterCategoryModel m_parameterCategoryModel;
    ControllerDeviceParameterModel         m_parameterModel;
    static constexpr int UNSELECTED = -1;
    int m_currentParameterInCategoryIndex{UNSELECTED};
signals:
    void parameterCategoryModelChanged(); //dummy, make qml property binding happy
    void parameterModelChanged();         //dummy, make qml property binding happy
};

} // namespace push2::qt

#endif