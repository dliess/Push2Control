#include "Midi1OutputManager.h"
#include "MidiMessage.h"
#include "IMidiOutMedium.h"
#include <iostream>
#include <chrono>
#include <loguru.hpp>

using namespace midi;

Midi1OutputManager::Midi1OutputManager(std::unique_ptr<IMidiOutMedium> pMedium):
    m_midi1Output(std::move(pMedium))
{}

void Midi1OutputManager::processBuffers() noexcept
{
    m_msgBuf.swap();
    if(m_msgBuf.inner().size())
    {
        for(auto& msg : m_msgBuf.inner())
        {
            m_midi1Output.send(msg);
        }
        return;
    }
    m_ccBuf.swap();
    if(m_ccBuf.inner().size())
    {
        for(auto& msg : m_ccBuf.inner())
        {
            m_midi1Output.send(MidiMessage(msg));
        }
        return;
    }
    m_sysExBuf.swap();
    if(m_sysExBuf.inner().size())
    {
        for(auto& msg : m_sysExBuf.inner())
        {
            m_midi1Output.sysEx(msg);
        }
    }
}

void Midi1OutputManager::noteOn(uint8_t channel, uint8_t note, uint8_t velocity) noexcept
{
    m_msgBuf.outerLocked( [&](auto& buf){
        buf.emplace_back(Message<NoteOn>(channel, note, velocity));
    } );
}

void Midi1OutputManager::noteOff(uint8_t channel, uint8_t note, uint8_t velocity) noexcept
{
    m_msgBuf.outerLocked( [&](auto& buf){
        buf.emplace_back(Message<NoteOff>(channel, note, velocity));
    } );
}

void Midi1OutputManager::pitchBend(uint8_t channel, int16_t value) noexcept
{
    m_msgBuf.outerLocked( [&](auto& buf){
        buf.emplace_back(Message<PitchBend>(channel, value));
    } );
}

void Midi1OutputManager::controlParameter(uint8_t channel, uint8_t ccId, uint8_t value) noexcept
{
    m_ccBuf.outerLocked( [&](auto& buf){
        buf.push_back(Message<ControlChange>(channel, ccId, value));
    } );
}

void Midi1OutputManager::sysEx(const std::vector<uint8_t>& msg) noexcept
{
    m_sysExBuf.outerLocked( [&](auto& buf){
        buf.push_back(msg);
    } );
}

void Midi1OutputManager::send(const MidiMessage& msg) noexcept
{
    mpark::visit(overload{
        [this](const Message<ControlChange>& ccMsg) {
            m_ccBuf.outerLocked( [&ccMsg](auto& buf){
                buf.push_back(ccMsg);
            } );
        },
        [this](auto&& otherMsg) { 
            m_msgBuf.outerLocked( [&otherMsg](auto& buf){
                buf.push_back(otherMsg);
            } );
        }
    }, msg);
}

