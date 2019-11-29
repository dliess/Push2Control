#ifndef PUSH2_QT_SOUND_PRESET_LIST_MODEL_H
#define PUSH2_QT_SOUND_PRESET_LIST_MODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include "MusicDeviceHolder.h"
#include <vector>

namespace push2::qt
{

class SoundPresetListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
public:
    SoundPresetListModel(MusicDeviceHolder::MusicDevices& rSoundDevices) noexcept;

    enum SoundPresetRoles {
        NameRole = Qt::UserRole + 1
    };
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int currentIndex() const noexcept;

    void onCurrentDeviceIndexChanged(int index);
    void onCurrentVoiceIndexChanged(int index);

    Q_INVOKABLE void resetToActualSoundPreset();
    Q_INVOKABLE void selectSoundPreset(int presedIdx);
    Q_INVOKABLE void resetParametersToZeroState();
    Q_INVOKABLE void storeAsSoundPreset(const QString& presetName);
    Q_INVOKABLE void deleteSoundPreset();

private:
    MusicDeviceHolder::MusicDevices& m_rSoundDevices;
    std::vector<std::string> m_actualSoundPresets;
    static constexpr int UNSET = -1;
    int m_currentSoundDeviceIdx{UNSET};
    int m_currentVoiceIdx{UNSET};
    void reset() noexcept;
signals:
    void currentIndexChanged();
};

} // namespace push2::qt

#endif