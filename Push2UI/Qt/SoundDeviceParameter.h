#ifndef SOUND_DEVICE_PARAMETER_H
#define SOUND_DEVICE_PARAMETER_H

#include <QObject>
#include "SoundDeviceParameterCategoryModel.h"
#include "SoundDeviceParameterModel.h"
#include "SoundPresetListModel.h"

struct MusicDeviceHolder;
class ParameterMapper;

namespace push2::qt
{

class SoundDeviceParameter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* parameterCategoryModel READ parameterCategoryModel NOTIFY parameterCategoryModelChanged)
    Q_PROPERTY(QAbstractListModel* parameterModel READ parameterModel NOTIFY parameterModelChanged)
    Q_PROPERTY(QAbstractListModel* soundPresetModel READ soundPresetModel NOTIFY soundPresetModelChanged)
    Q_PROPERTY(int currentCategoryIndex READ currentCategoryIndex)

public:
    explicit SoundDeviceParameter(MusicDeviceHolder& rMusicDeviceHolder,
                                  ParameterMapper&   rParameterMapper);
    QAbstractListModel* parameterCategoryModel();
    QAbstractListModel* parameterModel();
    QAbstractListModel* soundPresetModel();

    int currentCategoryIndex() const;
    Q_INVOKABLE void setCurrentCategoryIndex(int index);

    Q_INVOKABLE void incrementParameterValue(int paramIdxInCategory, int increment);

    Q_INVOKABLE void learnControl(int paramIdxInCategory);

public slots:
    void onCurrentDeviceIndexChanged(int index);
    void onCurrentVoiceIndexChanged(int index);

private:
    SoundDeviceParameterCategoryModel m_parameterCategoryModel;
    SoundDeviceParameterModel         m_parameterModel;
    SoundPresetListModel              m_soundPresetModel;

signals:
    void parameterCategoryModelChanged(); //dummy, make qml property binding happy
    void parameterModelChanged();         //dummy, make qml property binding happy
    void soundPresetModelChanged();       //dummy, make qml property binding happy
};

} // namespace push2::qt

#endif