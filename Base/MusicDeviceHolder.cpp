#include "MusicDeviceHolder.h"
#include "UsbMidiPortNotifier.h"
#include "UsbMidiIn.h"
#include "UsbMidiOut.h"
#include "MusicDeviceDescription.h"
#ifdef __INSERT_DUMMY_MIDI_DEVICES__
#include "MidiMediumDummy.h"
#endif
#include <memory>
#include <loguru.hpp>

MusicDeviceHolder::MusicDeviceHolder()
{
#ifndef __INSERT_DUMMY_MIDI_DEVICES__
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
#endif
}

void MusicDeviceHolder::addMidiInputMedium(std::unique_ptr<midi::IMidiInMedium> pMedium)
{
    const MusicDeviceId deviceId(pMedium->getDeviceName(), pMedium->getPortName());
    LOG_F(INFO, "Try to add midi input medium \"{}\"", deviceId.toStr());
    auto pMusicDevice = findMusicDevice(deviceId);
    if(!pMusicDevice)
    {
        pMusicDevice = createAndInsertMusicDevice(deviceId, pMedium->getDeviceName());
    } 
    pMusicDevice->addMidiIn(std::move(pMedium));
}

void MusicDeviceHolder::addMidiOutputMedium(std::unique_ptr<midi::IMidiOutMedium> pMedium)
{
    const MusicDeviceId deviceId(pMedium->getDeviceName(), pMedium->getPortName());
    LOG_F(INFO, "Try to add midi output medium \"{}\"", deviceId.toStr());
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
        deviceLoader.load(deviceId, *pDescr);
        if(pDescr->soundSection && pDescr->soundSection->parameters.size())
        {
            pSoundPresets = std::make_shared<SoundPresets>(pDescr->manufacturer, pDescr->productName);
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

#ifdef __INSERT_DUMMY_MIDI_DEVICES__
void MusicDeviceHolder::insertMusicDeviceDummies()
{
    midi::MidiMediumDummy dummy1("Moog Minitaur:Moog Minitaur MIDI 1", midi::IMidiMedium::Type::USB);
    midi::MidiMediumDummy dummy2("MIDIFACE 8x8:MIDIFACE 8x8 MIDI 1", midi::IMidiMedium::Type::USB);
    midi::MidiMediumDummy dummy3("TOUCHE_SE:TOUCHE_SE MIDI 1", midi::IMidiMedium::Type::USB);
    midi::MidiMediumDummy dummy4("Deluge:Deluge MIDI 1", midi::IMidiMedium::Type::USB);   
    midi::MidiMediumDummy dummy5("Arturia KeyStep 32:Arturia KeyStep 32 MIDI 1", midi::IMidiMedium::Type::USB);   
    midi::MidiMediumDummy dummy6("MODEL D:MODEL D MIDI 1", midi::IMidiMedium::Type::USB);   
    midi::MidiMediumDummy dummy7("Space Pedal:Space Pedal MIDI 1", midi::IMidiMedium::Type::USB);   
    midi::MidiMediumDummy dummy8("Launch Control XL:Launch Control XL MIDI 1", midi::IMidiMedium::Type::USB);
    midi::MidiMediumDummy dummy9("Seaboard BLOCK:Seaboard BLOCK MIDI 1", midi::IMidiMedium::Type::USB);
    midi::MidiMediumDummy dummy10("Buzzzy! polysynth:Buzzzy! polysynth MIDI 1", midi::IMidiMedium::Type::USB);
    midi::MidiMediumDummy dummy11("MicroBrute:MicroBrute MIDI 1", midi::IMidiMedium::Type::USB);
    midi::MidiMediumDummy dummy12("SUONOBUONO nABC:SUONOBUONO nABC MIDI 1", midi::IMidiMedium::Type::USB);
    midi::MidiMediumDummy dummy13("__JUST_4_TEST__MAM/MB33", midi::IMidiMedium::Type::USB);
    midi::MidiMediumDummy dummy14("__JUST_4_TEST__Korg/VolcaKeys", midi::IMidiMedium::Type::USB);   
    midi::MidiMediumDummy dummy15("__JUST_4_TEST__Korg/VolcaKick", midi::IMidiMedium::Type::USB);   
    midi::MidiMediumDummy dummy16("__JUST_4_TEST__Mitxela/Flash", midi::IMidiMedium::Type::USB);     
    midi::MidiMediumDummy dummy17("__JUST_4_TEST__Elektron/Octatrack_Mk1", midi::IMidiMedium::Type::USB);   
    midi::MidiMediumDummy dummy18("__JUST_4_TEST__Elektron/Analog4_Mk1", midi::IMidiMedium::Type::USB);   
    midi::MidiMediumDummy dummy19("__JUST_4_TEST__Elektron/AnalogRytm_Mk1", midi::IMidiMedium::Type::USB);

    addMidiInputMedium(dummy1.hijackInMedium());
    addMidiOutputMedium(dummy1.hijackOutMedium());
    addMidiInputMedium(dummy2.hijackInMedium());
    addMidiOutputMedium(dummy2.hijackOutMedium());
    addMidiInputMedium(dummy3.hijackInMedium());
    addMidiOutputMedium(dummy3.hijackOutMedium());
    addMidiInputMedium(dummy4.hijackInMedium());
    addMidiOutputMedium(dummy4.hijackOutMedium());
    addMidiInputMedium(dummy5.hijackInMedium());
    addMidiOutputMedium(dummy5.hijackOutMedium());
    addMidiInputMedium(dummy6.hijackInMedium());
    addMidiOutputMedium(dummy6.hijackOutMedium());
    addMidiInputMedium(dummy7.hijackInMedium());
    addMidiOutputMedium(dummy7.hijackOutMedium());
    addMidiInputMedium(dummy8.hijackInMedium());
    addMidiOutputMedium(dummy8.hijackOutMedium());
    addMidiInputMedium(dummy9.hijackInMedium());
    addMidiOutputMedium(dummy9.hijackOutMedium());
    addMidiInputMedium(dummy10.hijackInMedium());
    addMidiOutputMedium(dummy10.hijackOutMedium());
    addMidiInputMedium(dummy11.hijackInMedium());
    addMidiOutputMedium(dummy11.hijackOutMedium());
    addMidiInputMedium(dummy12.hijackInMedium());
    addMidiOutputMedium(dummy12.hijackOutMedium());
    addMidiInputMedium(dummy13.hijackInMedium());
    addMidiOutputMedium(dummy13.hijackOutMedium());
    addMidiInputMedium(dummy14.hijackInMedium());
    addMidiOutputMedium(dummy14.hijackOutMedium());
    addMidiInputMedium(dummy15.hijackInMedium());
    addMidiOutputMedium(dummy15.hijackOutMedium());
    addMidiInputMedium(dummy16.hijackInMedium());
    addMidiOutputMedium(dummy16.hijackOutMedium());
    addMidiInputMedium(dummy17.hijackInMedium());
    addMidiOutputMedium(dummy17.hijackOutMedium());
    addMidiInputMedium(dummy18.hijackInMedium());
    addMidiOutputMedium(dummy18.hijackOutMedium());
    addMidiInputMedium(dummy19.hijackInMedium());
    addMidiOutputMedium(dummy19.hijackOutMedium());
}
#endif
