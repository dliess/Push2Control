#ifndef UTILS_SETTINGS_H
#define UTILS_SETTINGS_H

#include <Meta.h>
#include <JsonCast.h>
#include <loguru.hpp>
#include <fstream>
#include <functional>
#include <experimental/filesystem>
#include <vector>
#include <iomanip>
#include "HomeDir.h"

namespace utils
{

template<typename Derived>
class Settings
{
public:
   void save(const std::string& relDirName, const std::string& fileName, const std::string& section) const noexcept;
   bool load(const std::string& relDirName, const std::string& fileName, const std::string& section) noexcept;
};

template<typename Derived>
void utils::Settings<Derived>::save(const std::string& relDirName,
                                    const std::string& fileName,
                                    const std::string& section) const noexcept
{
   const auto derived = reinterpret_cast<const Derived*>(this);
   nlohmann::json j;
   const std::string path = util::getLocalSettingsDir() + "/" + relDirName + "/" + fileName;
   {
      std::ifstream settingsFile(path);
      if(settingsFile.is_open())
      {
         try{
            settingsFile >> j;
         }catch(...){}
      }
   }
   try{
      // use tmpfilename to create settings to avoid corruption
      std::string fileNameTmp(path);
      fileNameTmp.append("_tmp_");
      std::ofstream settingsFile;
      settingsFile.exceptions ( std::ifstream::failbit );
      settingsFile.open(fileNameTmp);           
      j[section] = nlohmann::json(derived->getSettings());
      settingsFile << std::setw(4) << j;
      std::experimental::filesystem::rename(fileNameTmp, path);
   }
   catch(nlohmann::json::exception& e)
   {
      LOG_F(ERROR, "{} id: {}", e.what(), e.id);
   }
   catch(std::experimental::filesystem::filesystem_error& e)
   {
      LOG_F(ERROR, "Erorr in Settings Save: Filename '{}' Section '{}' exception: {}", path, section, e.what());
   }
   catch(std::exception& e)
   {
      LOG_F(ERROR, "Erorr in Settings Save: Filename '{}' Section '{}' exception: {}", path, section, e.what());
   }
}

template<typename Derived>
bool utils::Settings<Derived>::load(const std::string& relDirName,
                                    const std::string& fileName,
                                    const std::string& section) noexcept
{
   auto derived = reinterpret_cast<Derived*>(this);
   nlohmann::json j;
   try{
      std::ifstream settingsFile;
      const auto finalDir = util::getLocalSettingsDir() + "/" + relDirName;
      util::createDirsRecursive(finalDir);
      const std::string path = finalDir + "/" + fileName;
      settingsFile.open(path);
      if(settingsFile.fail())
      {
         LOG_F(INFO, "No settings file '{}' NOT found, its possibly the first run", path);
         return false;
      }
      settingsFile >> j;
      derived->setSettings(j[section].get<typename Derived::Settings>());
   }
   catch(nlohmann::json::exception& e){
      LOG_F(INFO, "{} id: {}", e.what(), e.id);
      return false;
   }
   catch(std::exception& e)
   {
      LOG_F(ERROR, "{}", e.what());
      return false;  
   }
   return true;
}

class SettingsFnCollection
{
public:
   inline void invokeLoaders();
   inline void invokeSavers();
   inline void addLoaderFn(std::function<void(void)> fn) noexcept;
   inline void addSaverFn(std::function<void(void)> fn) noexcept;
private:
   std::vector<std::function<void(void)>> m_loaders;
   std::vector<std::function<void(void)>> m_savers;
};

inline 
void SettingsFnCollection::invokeLoaders()
{
   for(auto& fn : m_loaders)
   {
      fn();
   }
}

inline 
void SettingsFnCollection::invokeSavers()
{
   for(auto& fn : m_savers)
   {
      fn();
   }
}

inline 
void SettingsFnCollection::addLoaderFn(std::function<void(void)> fn) noexcept
{
   m_loaders.push_back(fn);
}

inline 
void SettingsFnCollection::addSaverFn(std::function<void(void)> fn) noexcept
{
   m_savers.push_back(fn);
}


} // namespace utils


#endif