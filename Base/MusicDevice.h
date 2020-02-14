#ifndef MUSIC_DEVICE_H
#define MUSIC_DEVICE_H

#include "MusicDeviceId.h"
#include "SoundPresets.h"
#include "Midi1Input.h"
#include "MusicDeviceDescription.h"
#include "ParameterId.h"
#include "Settings.h"
#include "MidiInMsgHandler.h"
#include "MidiOutMsgHandler.h"
#include <optional>
#include <memory>
#include <vector>
#include <functional>

namespace midi { class IMidiInMedium; }
namespace midi { class Router; }
namespace midi { class Dumper; }

struct VoiceData
{
    std::vector<float> parameters;
    std::string        actualPreset;
};

namespace meta
{

template <>
inline auto registerMembers<VoiceData>()
{
   return members(
      member("parameters", &VoiceData::parameters),
      member("actualPreset", &VoiceData::actualPreset)
   );
}

}
struct MusicDevice : public utils::Settings<MusicDevice>
{
    MusicDevice(MusicDeviceId                           deviceId,
                std::shared_ptr<MusicDeviceDescription> descr,
                std::shared_ptr<SoundPresets> soundPresets) noexcept;
    ~MusicDevice() noexcept;

    void addMidiIn(std::unique_ptr<midi::IMidiInMedium> pMedium) noexcept;
    void addMidiOut(std::unique_ptr<midi::IMidiOutMedium> pMedium) noexcept;

    float getSoundParameterValue(uint32_t voiceId, uint32_t parameterId) const noexcept;
    void setSoundParameterValue(uint32_t voiceId, uint32_t parameterId, float value) noexcept;
    float getControllerParameterValue(uint32_t presetId, uint32_t parameterId) const noexcept;
    void quarterNotePer24Tick() noexcept;
    void startSequencer() noexcept;
    void stopSequencer() noexcept;

    std::vector<std::string> getSoundPresetList() const noexcept;
    std::string getActualSoundPresetName(int voiceIdx) const noexcept;
    void registerPresetListChangeCb(std::function<void()> cb) noexcept;
    void clearPresetListChangeCb() noexcept;
    void resetToActualSoundPreset(int voiceIdx) noexcept;
    void selectSoundPreset(int voiceIdx, const std::string& presetName) noexcept;
    void storeAsSoundPreset(int voiceIdx, const std::string& presetName) noexcept;
    void resetParametersToZeroState(int voiceIdx) noexcept;
    void deleteSoundPreset(int voiceIdx, const std::string& newSelectedPreset) noexcept;

    void noteOn(int voiceIndex, int note, float velocity) noexcept;
    void noteOff(int voiceIndex, int note, float velocity) noexcept;
    void pitchBend(int voiceIndex, float value) noexcept;


    using ParameterChangeCb = std::function<void(uint32_t voiceId, uint32_t parameterId, float value)>;
    void registerForControllerParameterChange(ParameterChangeCb cb) noexcept;

    void processMidiOutBuffers() noexcept;

    std::shared_ptr<MusicDeviceDescription> pDescr;
    std::shared_ptr<SoundPresets>           pSoundPresets;

    using SoundParams = std::vector<VoiceData>;
    // ============== Settings ===============
    using Settings = SoundParams;
    Settings getSettings() const noexcept { return m_soundParams; };
    void setSettings(const Settings& settings) noexcept { if(m_soundParams.size() == settings.size()) m_soundParams = settings; };
    // =======================================

    friend midi::Router;
    friend midi::Dumper;

private:
    MusicDeviceId                           m_deviceId;
    std::optional<MidiInMsgHandler>         m_midiInMsgHandler;
    std::optional<MidiOutMsgHandler>        m_midiOutHandler;
    SoundParams                             m_soundParams;
    std::vector<std::vector<float>>         m_controlParams;

    std::vector<ParameterChangeCb> m_controllerParameterChangeCbsAll;

    float getInitalParameterValue(uint32_t parameterId) const noexcept;
};

#endif