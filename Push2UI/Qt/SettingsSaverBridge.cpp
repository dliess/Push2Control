#include "SettingsSaverBridge.h"
#include "Settings.h"
#include "loguru.hpp"

push2::qt::SettingsSaverBridge::SettingsSaverBridge(utils::SettingsFnCollection& fnCollection) :
    QObject(),
    m_rFnCollection(fnCollection)
{}

void push2::qt::SettingsSaverBridge::saveSettings() noexcept
{
    LOG_F(INFO, "Saving settings");
    m_rFnCollection.invokeSavers();
}