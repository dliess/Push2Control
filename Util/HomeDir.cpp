#include "HomeDir.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <experimental/filesystem>

static std::string globalAppName;

void util::setAppName(std::string appName) noexcept
{
    globalAppName = appName;
}

std::string util::getHomeDir() noexcept
{
    struct passwd *pw = getpwuid(getuid());
    return pw->pw_dir;
}

std::string util::getLocalSettingsDir() noexcept
{
    return getHomeDir() + "/." + globalAppName;
}

void util::createDirsRecursive(const std::string& dirName)
{
    int pos = 0;
    do
    {
        pos = dirName.find("/", pos + 1);
        std::experimental::filesystem::create_directory(dirName.substr(0, pos));
    }while(pos != std::string::npos);
}
