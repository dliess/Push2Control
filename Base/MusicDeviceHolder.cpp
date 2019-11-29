#include "MusicDeviceHolder.h"
#include "UsbMidiPortNotifier.h"
#include "UsbMidiIn.h"
#include "UsbMidiOut.h"
#include "MusicDeviceDescription.h"
#include <memory>
#include <loguru.hpp>

MusicDeviceHolder::MusicDeviceHolder()
{
    midi::PortNotifiers::instance().inputs.registerNewPortCb([this](rtmidiadapt::PortIndex  index,
                                                        const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort){
        auto pMedium = std::make_unique<midi::UsbMidiIn>();
        if(!pMedium->openPort(index))
        {
            std::cerr << "failed to open input port" << std::endl;
            return;
        }
        addMidiInputMedium(std::move(pMedium));

    }, {{}, {"RtMidi", "Ableton Push 2", "Midi Through"}, false});
    midi::PortNotifiers::instance().outputs.registerNewPortCb([this](rtmidiadapt::PortIndex i,
                                             const rtmidiadapt::DeviceOnUsbPort& devOnUsbPort){
        auto pMedium = std::make_unique<midi::UsbMidiOut>();
        if(!pMedium->openPort(i))
        {
            std::cerr << "failed to open output port" << std::endl;
            return;
        }
        addMidiOutputMedium(std::move(pMedium));

    }, {{}, {"RtMidi", "Ableton Push 2", "Midi Through"}, false});
}

void MusicDeviceHolder::addMidiInputMedium(std::unique_ptr<midi::IMidiInMedium> pMedium)
{
    LOG_F(INFO, "adding midi input medium {}", pMedium->getDeviceName());
    const MusicDeviceId deviceId(pMedium->getDeviceName(), pMedium->getPortName());
    auto pMusicDevice = findMusicDevice(deviceId);
    if(!pMusicDevice)
    {
        pMusicDevice = createAndInsertMusicDevice(deviceId, pMedium->getDeviceName());
    } 
    pMusicDevice->addMidiIn(std::move(pMedium));
}

void MusicDeviceHolder::addMidiOutputMedium(std::unique_ptr<midi::IMidiOutMedium> pMedium)
{
    LOG_F(INFO, "adding midi output medium {}", pMedium->getDeviceName());
    const MusicDeviceId deviceId(pMedium->getDeviceName(), pMedium->getPortName());
    auto pMusicDevice = findMusicDevice(deviceId);
    if(!pMusicDevice)
    {
        pMusicDevice = createAndInsertMusicDevice(deviceId, pMedium->getDeviceName());
    }
    pMusicDevice->addMidiOut(std::move(pMedium));    
}

std::shared_ptr<MusicDevice> MusicDeviceHolder::findMusicDevice(const MusicDeviceId& deviceId) const noexcept
{
    auto itCntrlDev = controllerDevices.find(deviceId);
    if(itCntrlDev != controllerDevices.end())
    {
        return itCntrlDev->second;
    }
    auto itSndDev = soundDevices.find(deviceId);
    if(itSndDev != soundDevices.end())
    {
        return itSndDev->second;
    }
    return nullptr;
}

std::shared_ptr<MusicDevice> MusicDeviceHolder::createAndInsertMusicDevice(const MusicDeviceId&  deviceId,
                                                                           const std::string&    deviceName)
{
    const auto pSimilarMusicDevice = findMusicDevice(MusicDeviceId(deviceName, MusicDeviceId::ANY_PORT));
    std::shared_ptr<MusicDeviceDescription> pDescr;
    std::shared_ptr<SoundPresets> pSoundPresets;
    if(pSimilarMusicDevice)
    {
        pDescr = pSimilarMusicDevice->pDescr;
        pSoundPresets = pSimilarMusicDevice->pSoundPresets;
    }
    else
    {
        pDescr = std::make_shared<MusicDeviceDescription>();
        deviceLoader.load(deviceName, *pDescr);
        if(pDescr->soundSection && pDescr->soundSection->parameters.size())
        {
            pSoundPresets = std::make_shared<SoundPresets>(pDescr->manufacturer, pDescr->product_name);
        }
    }

    auto deviceType = MusicDeviceDescription::Type::Unknown;
    if(pDescr->controllerSection && pDescr->soundSection)
    {
        deviceType = MusicDeviceDescription::Type::Both;
    }
    else if(pDescr->controllerSection)
    {
        deviceType = MusicDeviceDescription::Type::ControllerDevice;
    }
    else if(pDescr->soundSection)
    {
        deviceType = MusicDeviceDescription::Type::SoundDevice;
    }

    auto pMusicDevice = std::make_shared<MusicDevice>(deviceId, std::move(pDescr), std::move(pSoundPresets));
    musicDevices[deviceId] = pMusicDevice;

    if(pMusicDevice->pDescr->reactsToTransportCommands && *pMusicDevice->pDescr->reactsToTransportCommands)
    {
        transportCmdDrains[deviceId] = pMusicDevice;
    }

    switch(deviceType)
    {
        case MusicDeviceDescription::Type::ControllerDevice:
        {
            controllerDevices[deviceId] = pMusicDevice;
            break;
        }
        case MusicDeviceDescription::Type::SoundDevice:
        {
            soundDevices[deviceId] = pMusicDevice;
            break;
        }
        case MusicDeviceDescription::Type::Both:
        {
            controllerDevices[deviceId] = pMusicDevice;
            soundDevices[deviceId] = pMusicDevice;
            break;
        }
        default:
        {
            LOG_F(ERROR, "ERROR: Wrong device type '{}'", MusicDeviceDescription::type2String(deviceType));
            break;
        }
    }
    return std::move(pMusicDevice);
}