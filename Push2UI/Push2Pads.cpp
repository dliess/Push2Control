#include "Push2Pads.h"
#include "FpLed.h"
#include "Push2LedOfWidget.h"
#include "Push2Topology.h"
#include "Push2Device.h"
#include "Midi.h"

using namespace push2;

uint8_t Pads::toNote(uint8_t x, uint8_t y) const noexcept
{
    const auto padDim = fp::Push2Topology::Led::getDim(fp::Push2Topology::Led::eBtnSilLed);
    const auto rowIdx = padDim.y - 1 - y;
    const auto colIdx = x;   

    switch(m_mode.load())
    {
        case Mode::InKey:
        {
            const auto idx = (rowIdx * NOTE_OFFSET_PER_ROW_INKEY) + colIdx + m_inKeyViewOffset;
            return m_rootNote + (m_octaveOffset * music::NUM_NOTES_PER_OCTAVE) +
                   m_scales[m_selectedScale].getNoteOffsetOf(idx);
        }
        case Mode::Chromatic: [[fallthrough]];
        default:
        {
            return (rowIdx * NOTE_OFFSET_PER_ROW_CHROMATIC) + colIdx + (m_octaveOffset * music::NUM_NOTES_PER_OCTAVE) + m_chromaticViewOffset;
        }
    }
}

bool Pads::isRootNote(uint8_t note) const noexcept
{
    return (note % music::NUM_NOTES_PER_OCTAVE) == (m_rootNote % music::NUM_NOTES_PER_OCTAVE);
}

Pads::Pads(push2device::Push2Device& rDevice) :
    m_rDevice(rDevice)
{
}

void Pads::updateLeds()
{
    for(uint8_t x = 0; x < 8; ++x)
    {
        for(uint8_t y = 0; y < 8; ++y)
        {
            const auto note = toNote(x, y);
            auto color = fp::Led::getRGB(fp::Led::Black);
            if(isRootNote(note))
            {
                color = fp::Led::getRGB(fp::Led::Magenta);
            }
            else if(m_scales[m_selectedScale].contains(note - m_rootNote))
            {
                color = fp::Led::getRGB(fp::Led::LightGray);
            }
            m_rDevice.setLedOfScene(this, {fp::Push2Topology::Led::eBtnSilLed, x, y}, color);
        }
    }
}

void Pads::activate() noexcept
{
    m_rDevice.registerCB(static_cast<fp::Button3d::CallbackIf&>(*this), fp::Widget(fp::Push2Topology::Button3d::eBtnSil, fp::IdxAll, fp::IdxAll));
    m_rDevice.reserveLed(this, fp::Widget(fp::Push2Topology::Led::eBtnSilLed, fp::IdxAll, fp::IdxAll), {0, 0, 0});
}

void Pads::deActivate() noexcept
{
    m_rDevice.releaseLed(this, fp::Widget(fp::Push2Topology::Led::eBtnSilLed, fp::IdxAll, fp::IdxAll));
    m_rDevice.unRegisterCB(static_cast<fp::Button3d::CallbackIf&>(*this), fp::Widget(fp::Push2Topology::Button3d::eBtnSil, fp::IdxAll, fp::IdxAll)); 
}

midi::IMidiMedium::Type Pads::getType() const
{
    return midi::IMidiMedium::USB;
}


std::string Pads::getPortName() const
{
    return "Push2PadsPort";
}


std::string Pads::getDeviceName() const
{
    return "Push2Pads";
}


void Pads::registerCallback(Callback cb)
{
    m_callbacks.push_back(cb);
}


void Pads::update()
{

}


void Pads::onPressStateChange(const fp::Button3d::StateData& data, const fp::Widget& w)
{
    if(fp::Button3d::Pressed == data.pressState)
    {
        m_pressedBtns.set(w.coord, data.velocity);
    }
    else
    {
        m_pressedBtns.clear(w.coord);
    }
    pressStateChange(data, w);
}

void Pads::pressStateChange(const fp::Button3d::StateData& data, const fp::Widget& w)
{
    //std::cout << "x: " << w.coord.x << " y: " << std::endl;
    std::vector<uint8_t> msg(3);
    switch(data.pressState)
    {
        case fp::Button3d::Pressed:
        {
            msg[0] = midi::Command<midi::NoteOn>(m_channel);
            break;
        }
        case fp::Button3d::Released: [[fallthrough]];
        default:
        {
            msg[0] = midi::Command<midi::NoteOff>(m_channel);
            break;
        }
    }
    msg[1] = toNote(w.coord.x, w.coord.y);
    msg[2] = data.velocity;
    for(auto& cb : m_callbacks) { cb(0, msg); }
}


void Pads::onPositionEvents(const uint32_t& pressure, const fp::Widget& w)
{
    std::vector<uint8_t> msg(3);
    msg[0] = midi::Command<midi::AfterTouchPoly>(m_channel);
    msg[1] = toNote(w.coord.x, w.coord.y);
    msg[2] = pressure;
    for(auto& cb : m_callbacks) { cb(0, msg); }
}


void Pads::setMusicScale(music::Scales::Type musicScale)
{
    m_pressedBtns.forEach([this](const fp::Vector2d<uint8_t>& coord, uint8_t velocity){
        pressStateChange({Button3d::PressState::Released, velocity}, fp::Widget(fp::Push2Topology::Button3d::eBtnSil, coord));
    });
    m_selectedScale = musicScale;
    m_pressedBtns.forEach([this](const fp::Vector2d<uint8_t>& coord, uint8_t velocity){
        pressStateChange({Button3d::PressState::Pressed, velocity}, fp::Widget(fp::Push2Topology::Button3d::eBtnSil, coord));
    });
    updateLeds();
}

music::Scales::Type Pads::getMusicScale() const
{
    return m_selectedScale;
}

void Pads::setRootNote(music::Note newRootNote)
{
    m_pressedBtns.forEach([this](const fp::Vector2d<uint8_t>& coord, uint8_t velocity){
        pressStateChange({Button3d::PressState::Released, velocity}, fp::Widget(fp::Push2Topology::Button3d::eBtnSil, coord));
    });
    m_rootNote = newRootNote;
    m_pressedBtns.forEach([this](const fp::Vector2d<uint8_t>& coord, uint8_t velocity){
        pressStateChange({Button3d::PressState::Pressed, velocity}, fp::Widget(fp::Push2Topology::Button3d::eBtnSil, coord));
    });
    updateLeds();
}

music::Note Pads::getRootNote() const
{
    return m_rootNote;
}

void Pads::toggleMode()
{
    Mode newMode(m_mode);
    switch(m_mode)
    {
        case Pads::Mode::Chromatic: newMode = Pads::Mode::InKey;     break;
        case Pads::Mode::InKey:     newMode = Pads::Mode::Chromatic; break;
        default: break;
    }
    m_pressedBtns.forEach([this](const fp::Vector2d<uint8_t>& coord, uint8_t velocity){
        pressStateChange({Button3d::PressState::Released, velocity}, fp::Widget(fp::Push2Topology::Button3d::eBtnSil, coord));
    });
    m_mode = newMode;
    m_pressedBtns.forEach([this](const fp::Vector2d<uint8_t>& coord, uint8_t velocity){
        pressStateChange({Button3d::PressState::Pressed, velocity}, fp::Widget(fp::Push2Topology::Button3d::eBtnSil, coord));
    });

    updateLeds();
}

Pads::Mode Pads::getMode() const
{
    return m_mode;
}

void Pads::setOctave(int octave)
{
    auto toSet = std::min((127 / music::NUM_NOTES_PER_OCTAVE), std::max(0, octave));
    if(toSet != m_octaveOffset)
    {
        m_pressedBtns.forEach([this](const fp::Vector2d<uint8_t>& coord, uint8_t velocity){
            pressStateChange({Button3d::PressState::Released, velocity}, fp::Widget(fp::Push2Topology::Button3d::eBtnSil, coord));
        });
        m_octaveOffset = toSet;
        m_pressedBtns.forEach([this](const fp::Vector2d<uint8_t>& coord, uint8_t velocity){
            pressStateChange({Button3d::PressState::Pressed, velocity}, fp::Widget(fp::Push2Topology::Button3d::eBtnSil, coord));
        });
    }
}

int Pads::getOctave() const
{
    return m_octaveOffset;
}
