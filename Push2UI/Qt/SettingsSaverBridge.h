#ifndef SETTINGS_SAVER_BRIDGE_H
#define SETTINGS_SAVER_BRIDGE_H

#include <QObject>

namespace utils { class SettingsFnCollection; }

namespace push2::qt
{

class SettingsSaverBridge : public QObject
{
    Q_OBJECT
public:
    SettingsSaverBridge(utils::SettingsFnCollection& fnCollection);
    Q_INVOKABLE void saveSettings() noexcept;
private:
    utils::SettingsFnCollection& m_rFnCollection;
};

} // namespace push2::qt

#endif