#ifndef MUSIC_DEVICE_HOLDER_H
#define MUSIC_DEVICE_HOLDER_H

#include <functional>
#include <memory>

#include "KeyValueVector.h"
#include "MusicDevice.h"

namespace base { class MusicDeviceFactory; }

struct MusicDeviceHolder
{
   MusicDeviceHolder();
   inline std::shared_ptr<MusicDevice>& controllerDevice(int index);
   inline std::shared_ptr<MusicDevice>& soundDevice(int index);
   inline const std::shared_ptr<MusicDevice>& controllerDevice(int index) const;
   inline const std::shared_ptr<MusicDevice>& soundDevice(int index) const;

   class MusicDevices
      : public util::KeyValueVector<MusicDeviceId, std::shared_ptr<MusicDevice>>
   {
   public:
      using AddRemoveCb = std::function<void(MusicDeviceId)>;
      inline void registerForAdd(AddRemoveCb cb) noexcept;
      inline void registerForRemove(AddRemoveCb cb) noexcept;

      friend base::MusicDeviceFactory;

   private:
      std::vector<AddRemoveCb> m_addedCb;
      std::vector<AddRemoveCb> m_removedCb;
      inline void invokeAddCbs(const MusicDeviceId& id);
      inline void invokeRemoveCbs(const MusicDeviceId& id);
   };

   MusicDevices musicDevices;
   MusicDevices controllerDevices;
   MusicDevices soundDevices;
   MusicDevices transportCmdDrains;

   inline void processMidiInBuffers();
   inline void processMidiOutBuffers();
};

inline std::shared_ptr<MusicDevice>& MusicDeviceHolder::controllerDevice(
   int index)
{
   return controllerDevices.atIdx(index).second;
};
inline std::shared_ptr<MusicDevice>& MusicDeviceHolder::soundDevice(int index)
{
   return soundDevices.atIdx(index).second;
};
inline const std::shared_ptr<MusicDevice>& MusicDeviceHolder::controllerDevice(
   int index) const
{
   return controllerDevices.atIdx(index).second;
};
inline const std::shared_ptr<MusicDevice>& MusicDeviceHolder::soundDevice(
   int index) const
{
   return soundDevices.atIdx(index).second;
};

inline void MusicDeviceHolder::processMidiInBuffers()
{
   for (auto& musicDevice : musicDevices)
   {
      musicDevice.second->processMidiInBuffers();
   }
}

inline void MusicDeviceHolder::processMidiOutBuffers()
{
   for (auto& musicDevice : musicDevices)
   {
      musicDevice.second->processMidiOutBuffers();
   }
}

inline void MusicDeviceHolder::MusicDevices::registerForAdd(
   AddRemoveCb cb) noexcept
{
   m_addedCb.push_back(cb);
}

inline void MusicDeviceHolder::MusicDevices::registerForRemove(
   AddRemoveCb cb) noexcept
{
   m_removedCb.push_back(cb);
}

inline void MusicDeviceHolder::MusicDevices::invokeAddCbs(
   const MusicDeviceId& id)
{
   for (auto& cb : m_addedCb) cb(id);
}
inline void MusicDeviceHolder::MusicDevices::invokeRemoveCbs(
   const MusicDeviceId& id)
{
   for (auto& cb : m_removedCb) cb(id);
}

#endif