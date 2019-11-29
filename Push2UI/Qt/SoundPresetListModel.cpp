#include "SoundPresetListModel.h"
#include "Alphanum.hpp"

push2::qt::SoundPresetListModel::SoundPresetListModel(MusicDeviceHolder::MusicDevices& rSoundDevices) noexcept :
    m_rSoundDevices(rSoundDevices)
{
}

QHash<int, QByteArray> push2::qt::SoundPresetListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    return roles;
}

int push2::qt::SoundPresetListModel::rowCount(const QModelIndex&) const
{
    return m_actualSoundPresets.size();
}

QVariant push2::qt::SoundPresetListModel::data(const QModelIndex &index, int role) const
{
    switch(role)
    {
        case NameRole:
        {
            if(index.row() < 0 || index.row() >= m_actualSoundPresets.size()) break;
            return QString(m_actualSoundPresets[index.row()].c_str());
        }
        default: break;
    }
    return QVariant();
}

int push2::qt::SoundPresetListModel::currentIndex() const noexcept
{
    if(UNSET == m_currentSoundDeviceIdx) return -1;
    if(UNSET == m_currentVoiceIdx) return -1;
    const std::string actPresetName = m_rSoundDevices.atIdx(m_currentSoundDeviceIdx).second->getActualSoundPresetName(m_currentVoiceIdx);
    for(int i = 0; i < m_actualSoundPresets.size(); ++i)
    {
        if(actPresetName == m_actualSoundPresets[i])
        {
            return i;
        }
    }
    return -1;
}

void push2::qt::SoundPresetListModel::onCurrentDeviceIndexChanged(int index)
{
    if(m_currentSoundDeviceIdx == index) return;
    if(m_currentSoundDeviceIdx != UNSET)
    {
        m_rSoundDevices.atIdx(m_currentSoundDeviceIdx).second->clearPresetListChangeCb();
    }
    m_currentSoundDeviceIdx = index;
    if(m_currentSoundDeviceIdx != UNSET)
    {
        m_rSoundDevices.atIdx(m_currentSoundDeviceIdx).second->registerPresetListChangeCb(
            std::bind(&SoundPresetListModel::reset, this)    
        );
    }
    reset();
}

void push2::qt::SoundPresetListModel::onCurrentVoiceIndexChanged(int index)
{
    if(m_currentVoiceIdx == index) return;
    m_currentVoiceIdx = index;
    emit currentIndexChanged();
}

void push2::qt::SoundPresetListModel::reset() noexcept
{
    QAbstractListModel::beginResetModel();
    m_actualSoundPresets.clear();
    if(UNSET != m_currentSoundDeviceIdx)
    {
        for(auto& presetName : m_rSoundDevices.atIdx(m_currentSoundDeviceIdx).second->getSoundPresetList())
        {
            m_actualSoundPresets.push_back(presetName);
        }
        std::sort(m_actualSoundPresets.begin(), m_actualSoundPresets.end(), [](const std::string& lhs, const std::string& rhs){
            return (0 > doj::alphanum_comp(lhs, rhs));
        } );
    }
    QAbstractListModel::endResetModel();
}

void push2::qt::SoundPresetListModel::resetToActualSoundPreset()
{
    if(UNSET == m_currentSoundDeviceIdx) return;
    if(UNSET == m_currentVoiceIdx) return;
    m_rSoundDevices.atIdx(m_currentSoundDeviceIdx).second->resetToActualSoundPreset(m_currentVoiceIdx);
}

void push2::qt::SoundPresetListModel::selectSoundPreset(int presedIdx)
{
    if(UNSET == m_currentSoundDeviceIdx) return;
    if(UNSET == m_currentVoiceIdx) return;
    if(presedIdx < 0 || presedIdx >= m_actualSoundPresets.size()) return;
    m_rSoundDevices.atIdx(m_currentSoundDeviceIdx).second->selectSoundPreset(m_currentVoiceIdx, m_actualSoundPresets[presedIdx]);
}

void push2::qt::SoundPresetListModel::resetParametersToZeroState()
{
    if(UNSET == m_currentSoundDeviceIdx) return;
    if(UNSET == m_currentVoiceIdx) return;
    m_rSoundDevices.atIdx(m_currentSoundDeviceIdx).second->resetParametersToZeroState(m_currentVoiceIdx);
}

void push2::qt::SoundPresetListModel::storeAsSoundPreset(const QString& presetName)
{
    if(UNSET == m_currentSoundDeviceIdx) return;
    if(UNSET == m_currentVoiceIdx) return;
    m_rSoundDevices.atIdx(m_currentSoundDeviceIdx).second->storeAsSoundPreset(m_currentVoiceIdx, presetName.toUtf8().constData());
    emit currentIndexChanged();
}

void push2::qt::SoundPresetListModel::deleteSoundPreset()
{
    if(UNSET == m_currentSoundDeviceIdx) return;
    if(UNSET == m_currentVoiceIdx) return;
    const std::string actPresetName = m_rSoundDevices.atIdx(m_currentSoundDeviceIdx).second->getActualSoundPresetName(m_currentVoiceIdx);
    auto iter = std::find(m_actualSoundPresets.begin(), m_actualSoundPresets.end(), actPresetName);
    if(iter == m_actualSoundPresets.end()) return;
    std::string newSelectedPreset;
    if(iter != m_actualSoundPresets.begin())
    {
        newSelectedPreset = *(iter - 1);
    }
    else if(iter + 1 != m_actualSoundPresets.end())
    {
        newSelectedPreset = *(iter + 1);
    }
    m_rSoundDevices.atIdx(m_currentSoundDeviceIdx).second->deleteSoundPreset(m_currentVoiceIdx, newSelectedPreset);
    emit currentIndexChanged();
}
