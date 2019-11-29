#ifndef SELECTED_SOUND_DEVICE_H
#define SELECTED_SOUND_DEVICE_H

#include <utility>
#include <functional>
#include <vector>

class SelectedSoundDevice
{
public:
    inline bool isUnselected() const noexcept;
    inline void set(int musicDeviceIdx, int m_voiceIdx) noexcept;
    inline void setDeviceIdx(int musicDeviceIdx) noexcept;
    inline void setVoiceIdx(int voiceIdx) noexcept;
    inline std::pair<int,int> get() const noexcept;
    inline void registerForChangeNotification(std::function<void()> cb) noexcept; // TODO clear if unused
    inline void registerForDeviceChangeNotification(std::function<void()> cb) noexcept;
    inline void registerForVoiceChangeNotification(std::function<void()> cb) noexcept;
private:
    static constexpr int UNSELECTED = -1;
    int m_musicDeviceIndex{UNSELECTED};
    int m_voiceId{UNSELECTED};
    std::vector<std::function<void()>> m_bothCangeCbs;
    std::vector<std::function<void()>> m_deviceCangeCbs;
    std::vector<std::function<void()>> m_voiceCangeCbs;
};

inline
bool SelectedSoundDevice::isUnselected() const noexcept
{
    return -1 == m_musicDeviceIndex;
}

inline 
void SelectedSoundDevice::set(int musicDeviceIdx, int voiceIdx) noexcept
{
    setDeviceIdx(musicDeviceIdx);
    setVoiceIdx(voiceIdx);
    if(m_musicDeviceIndex != musicDeviceIdx ||
       m_voiceId != voiceIdx)
    {
        for(auto& cb : m_bothCangeCbs) cb();
    }
}

inline
void SelectedSoundDevice::setDeviceIdx(int musicDeviceIdx) noexcept
{
    if(m_musicDeviceIndex != musicDeviceIdx)
    {
        m_musicDeviceIndex = musicDeviceIdx;
        for(auto& cb : m_deviceCangeCbs) cb();
    }
}

inline
void SelectedSoundDevice::setVoiceIdx(int voiceIdx) noexcept
{
    if(m_voiceId != voiceIdx)
    {
        m_voiceId = voiceIdx;
        for(auto& cb : m_voiceCangeCbs) cb();
    }
}

inline 
std::pair<int,int> SelectedSoundDevice::get() const noexcept
{
    return std::make_pair(m_musicDeviceIndex, m_voiceId);
}

inline 
void SelectedSoundDevice::registerForChangeNotification(std::function<void()> cb) noexcept
{
    m_bothCangeCbs.push_back(cb);
}

inline
void SelectedSoundDevice::registerForDeviceChangeNotification(std::function<void()> cb) noexcept
{
    m_deviceCangeCbs.push_back(cb);
}

inline
void SelectedSoundDevice::registerForVoiceChangeNotification(std::function<void()> cb) noexcept
{
    m_voiceCangeCbs.push_back(cb);
}

#endif