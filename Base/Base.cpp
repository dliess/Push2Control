#include "Base.h"
#include "ThreadedLoop.h"
#include <loguru.hpp>

base::Base::Base() noexcept :
    instruments(),
    musicDeviceHolder(),
    musicDeviceFactory(musicDeviceHolder, instruments),
    tempoHandler(musicDeviceHolder.musicDevices),
    m_midiOutThreadLoop([this](){threadLoop();})
{
    // TODO: Remove Dummy
    instruments.load("relDir", "filename", "section");
}

void base::Base::threadLoop() // TODO: refactor to
{
    auto start = std::chrono::high_resolution_clock::now();
    threadFunction();
    auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::nanoseconds diff = end - start;
    const std::chrono::nanoseconds PERIOD(1000000);
    if(diff < PERIOD)
    {
        std::this_thread::sleep_for(PERIOD - diff);
    }
}

void base::Base::threadFunction()
{
    for(auto& musicDevice : musicDeviceHolder.musicDevices){
        musicDevice.second->processMidiInBuffers();
    }
    tempoHandler.nextTimeSlot();
    for(auto& musicDevice : musicDeviceHolder.musicDevices){
        musicDevice.second->processMidiOutBuffers();
    }
}
