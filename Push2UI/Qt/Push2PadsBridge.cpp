#include "Push2PadsBridge.h"
#include "Push2Pads.h"

push2::qt::PadsBridge::PadsBridge(push2::Pads& rPads, QObject* parent) :
    QObject(parent),
    m_rPads(rPads)
{}

void push2::qt::PadsBridge::activate()
{
    m_rPads.activate();
}

void push2::qt::PadsBridge::deActivate()
{
    m_rPads.deActivate();
}

int push2::qt::PadsBridge::rootNote() const
{
    return m_rPads.getRootNote();
}  

void push2::qt::PadsBridge::setRootNote(MusicNote::Value newRootNote)
{
    m_rPads.setRootNote(static_cast<music::Note>(newRootNote));
    emit rootNoteChanged();
}

int push2::qt::PadsBridge::musicScale() const
{
    return m_rPads.getMusicScale();
}

void push2::qt::PadsBridge::setMusicScale(MusicScales::Value musicScale)
{
    m_rPads.setMusicScale(static_cast<music::Scales::Type>(musicScale));
    emit musicScaleChanged();
}

void push2::qt::PadsBridge::toggleMode()
{
    m_rPads.toggleMode();
    emit modeChanged();
}

int push2::qt::PadsBridge::mode() const
{
    return static_cast<int>(m_rPads.getMode());
}

int push2::qt::PadsBridge::octave() const
{
    return m_rPads.getOctave();
}

void push2::qt::PadsBridge::incOctave()
{
   m_rPads.setOctave(m_rPads.getOctave() + 1);
   emit octaveChanged();
}

void push2::qt::PadsBridge::decOctave()
{
   m_rPads.setOctave(m_rPads.getOctave() - 1);
   emit octaveChanged();    
}