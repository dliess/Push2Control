#ifndef UTIL_HOME_DIR_H
#define UTIL_HOME_DIR_H

#include <string>

namespace util
{

void setAppName(std::string appName) noexcept;
std::string getHomeDir() noexcept;
std::string getLocalSettingsDir() noexcept;
void createDirsRecursive(const std::string& dirName);

} // namespace util

#endif