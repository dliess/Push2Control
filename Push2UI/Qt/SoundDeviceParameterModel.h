#ifndef SOUND_DEVICE_PARAMETER_MODEL_H
#define SOUND_DEVICE_PARAMETER_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QHash>
#include <QTimer>

struct MusicDeviceHolder;
class ParameterMapper;

namespace push2::qt
{

class SoundDeviceParameterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MidiPortRoles {
        NameRole = Qt::UserRole + 1,
        Value,
        Type,
        MappingCurveRole,
        ExtControlLearnState
    };

    SoundDeviceParameterModel(MusicDeviceHolder& rMusicDeviceHolder,
                              ParameterMapper&   rParameterMapper,
                              QObject*           parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    int currentCategoryIndex() const;
    void setCurrentCategoryIndex(int index);

    void onCurrentDeviceIndexChanged(int index);
    void onCurrentVoiceIndexChanged(int index);

    void incrementParameterValue(int paramIdxInCategory, int increment);
    void learnControl(int paramIdxInCategory);

private:
    MusicDeviceHolder& m_rMusicDeviceHolder;
    ParameterMapper&   m_rParameterMapper;
    QTimer             m_updateTimer;
    static constexpr int UNSELECTED = -1;
    std::vector<int> m_currentCategoryIndexes;
    int m_currentDeviceIndex{UNSELECTED};
    int m_currentVoiceIndex{UNSELECTED};

    int parameterId(int paramIdxInCategory) const;
};

} // namespace push2::qt
#endif