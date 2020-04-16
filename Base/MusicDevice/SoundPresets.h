#ifndef SOUND_PRESETS_H
#define SOUND_PRESETS_H

#include <string>
#include <vector>
#include <loguru.hpp>
#include <unordered_map>
#include <mutex>
#include "Settings.h"
#include "Meta.h"

class SoundPresets : public utils::Settings<SoundPresets>
{
public:
   SoundPresets(std::string manufacturer, std::string product) noexcept;
   ~SoundPresets();
   std::vector<std::string> getSoundPresetList() const noexcept;
   const std::vector<float>& at(const std::string& presetName) const;
   bool hasSoundPreset(const std::string& presetName) const noexcept;
   void saveSoundPreset(const std::string& presetName,
                        const std::vector<float>& voiceParams) noexcept;
   std::string incrementNameIdx(const std::string& presetName) const noexcept;
   void deleteSoundPreset(const std::string& presetName) noexcept;

   void registerPresetListChangeCb(std::function<void()> cb) noexcept;
   void clearPresetListChangeCb() noexcept;

   using Presets = std::unordered_map<std::string, std::vector<float>>;
   // ============== Settings ===============
   using Settings = Presets;
   Settings getSettings() const noexcept { return m_presets; };
   void setSettings(const Settings& settings) noexcept { m_presets = settings; };
   // =======================================

private:
   Presets m_presets;
   std::string m_manufacturer;
   std::string m_product;
   std::function<void()> m_changeCb;
   std::mutex            m_changedCbMutex;
   std::string outDirName() const noexcept;
   std::string outFileName() const noexcept;
   inline static const std::string SETTING_FILE_SECTION = "sound-preset-data";
};

#endif