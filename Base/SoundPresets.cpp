#include "SoundPresets.h"

SoundPresets::SoundPresets(std::string manufacturer, std::string product) noexcept :
   m_manufacturer(std::move(manufacturer)),
   m_product(std::move(product))
{
   utils::Settings<SoundPresets>::load(outDirName(), outFileName(), SETTING_FILE_SECTION);
}

SoundPresets::~SoundPresets()
{
   utils::Settings<SoundPresets>::save(outDirName(), outFileName(), SETTING_FILE_SECTION);
}

std::vector<std::string> SoundPresets::getSoundPresetList() const noexcept
{
   std::vector<std::string> ret;
   for(auto& e : m_presets) {
      ret.push_back(e.first);
   }
   return ret;
}

const std::vector<float>& SoundPresets::at(const std::string& presetName) const
{
   const auto it = m_presets.find(presetName);
   if(it == m_presets.end()) throw std::out_of_range("No preset found with this name");
   return it->second;
}

bool SoundPresets::hasSoundPreset(const std::string& presetName) const noexcept
{
   return m_presets.find(presetName) != m_presets.end();
}

void SoundPresets::saveSoundPreset(const std::string&        presetName,
                                   const std::vector<float>& voiceParams) noexcept
{
   const bool isNew = m_presets.find(presetName) == m_presets.end();
   m_presets[presetName] = voiceParams;
   if(isNew)
   {
      std::lock_guard<std::mutex> lock(m_changedCbMutex);
      if(m_changeCb) m_changeCb();
   }
}

std::string SoundPresets::incrementNameIdx(const std::string& presetName) const noexcept
{
   static const std::string DELIM = "~";
   auto n = presetName.rfind(DELIM);
   if(n == std::string::npos)
   {
      return presetName + DELIM + "1";
   }
   else
   {
      const std::string baseName = presetName.substr(0, n);;
      try{
         int index = std::stoi(presetName.substr(n + 1));
         while(m_presets.end() != m_presets.find(baseName + DELIM + std::to_string(++index)));
         return baseName + DELIM + std::to_string(index);
      }catch(std::invalid_argument& e){
         LOG_F(ERROR, "Could not get index of preset file '{}', {}", presetName, e.what());
         return incrementNameIdx(baseName + "_");
      }
   }
}

void SoundPresets::deleteSoundPreset(const std::string& presetName) noexcept
{
   m_presets.erase(presetName);
   std::lock_guard<std::mutex> lock(m_changedCbMutex);
   if(m_changeCb) m_changeCb();
}

std::string SoundPresets::outDirName() const noexcept
{
   std::string str = "Presets/Devices/" +  m_manufacturer;
   std::replace(str.begin(), str.end(), ' ', '_');
   return str;
}

std::string SoundPresets::outFileName() const noexcept
{
   std::string str = m_product + ".json";
   std::replace(str.begin(), str.end(), ' ', '_');
   return str;
}

void SoundPresets::registerPresetListChangeCb(std::function<void()> cb) noexcept
{
   std::lock_guard<std::mutex> lock(m_changedCbMutex);
   m_changeCb = cb;
}

void SoundPresets::clearPresetListChangeCb() noexcept
{
   std::lock_guard<std::mutex> lock(m_changedCbMutex);
   m_changeCb = nullptr;
}
