#ifndef CONTROLLER_DEVICE_PARAMETER_MODEL_H
#define CONTROLLER_DEVICE_PARAMETER_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QHash>
#include <QTimer>

struct MusicDeviceHolder;
class ParameterMapper;

namespace push2::qt
{

class ControllerDeviceParameterModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum MidiPortRoles {
        NameRole = Qt::UserRole + 1,
        Value,
        Type,
        MappingCurveRole
    };

    ControllerDeviceParameterModel(MusicDeviceHolder& rMusicDeviceHolder,
                                   ParameterMapper&   rParameterMapper,
                                   QObject*           parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    int currentCategoryIndex() const;
    void setCurrentCategoryIndex(int index);

    void incrementValueMapOutAtMaxAmount(int parameterInCategoryIndex, float increment);
    void incrementValueMapOutAtMinAmount(int parameterInCategoryIndex, float increment);
    void incrementValueMapBendAmount(int parameterInCategoryIndex, float increment);

    void onCurrentDeviceIndexChanged(int index);
    void onCurrentPresetIndexChanged(int index);

private:
    MusicDeviceHolder& m_rMusicDeviceHolder;
    ParameterMapper&   m_rParameterMapper;
    QTimer             m_updateTimer;
    static constexpr int UNSELECTED = -1;
    std::vector<int> m_currentCategoryIndexes;
    int m_currentDeviceIndex{UNSELECTED};
    int m_currentPresetIndex{UNSELECTED};
    int parameterId(int paramIdxInCategory) const;
};

} // namespace push2::qt
#endif