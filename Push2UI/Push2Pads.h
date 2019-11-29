#ifndef PUSH2_PADS_H
#define PUSH2_PADS_H

#include "IMidiInMedium.h"
#include "FpButton3d.h"
#include "MusicScale.h"
#include "Push2Device.h"
#include <vector>
#include <cstdint>
#include <array>
#include <atomic>

namespace push2
{

class Pads : public midi::IMidiInMedium,
             public fp::Button3d::CallbackIf,
             public fp::Led::ILedScene
{
public:
    Pads(push2device::Push2Device& rDevice);
    Pads(const Pads&) = delete;
    Pads& operator=(const Pads&) = delete;

    void updateLeds();

    midi::IMidiMedium::Type getType() const override;
    std::string getPortName() const override;
    std::string getDeviceName() const override;
    void registerCallback(Callback cb) override;
    void update() override;

    void onGotRevealed(const Widget& w) final {}
    void onGotHidden(const Widget& w) final {}


    void activate() noexcept;
    void deActivate() noexcept;

    void onPressStateChange(const fp::Button3d::StateData& data, const fp::Widget& w) override;
    void onPositionEvents(const uint32_t& pressure, const fp::Widget& w) override;

    void setMusicScale(music::Scales::Type musicScale);
    music::Scales::Type getMusicScale() const;
    void setRootNote(music::Note newRootNote);
    music::Note getRootNote() const;
    enum class Mode
    {
        InKey = 0,
        Chromatic
    };
    void toggleMode();
    Mode getMode() const;

    void setOctave(int octave);
    int getOctave() const;

private:
    push2device::Push2Device& m_rDevice;
    std::vector<Callback>     m_callbacks;
    uint8_t                   m_channel{1};

    std::atomic<uint8_t> m_chromaticViewOffset{0};
    std::atomic<uint8_t> m_inKeyViewOffset{0};
    std::atomic<uint8_t> m_octaveOffset{2};
    std::atomic<music::Note> m_rootNote{music::Note::C};
    std::atomic<music::Scales::Type> m_selectedScale{music::Scales::Major};
    music::Scales m_scales;
    std::atomic<Mode> m_mode{Mode::InKey};
    class PressedPadsBitfield
    {
    public:
        inline void set(const fp::Vector2d<uint8_t>& coord, uint8_t vel) noexcept;
        inline void clear(const fp::Vector2d<uint8_t>& coord) noexcept;
        template <typename CbFunc>
        void forEach(CbFunc&& cbFunc) noexcept;
    private:
        std::array<std::array<uint8_t, 8>, 8> m_rows{0};
    };
    PressedPadsBitfield m_pressedBtns;

    uint8_t toNote(uint8_t x, uint8_t y) const noexcept;
    bool isRootNote(uint8_t note) const noexcept;
    void pressStateChange(const fp::Button3d::StateData& data, const fp::Widget& w);
    static const uint8_t NOTE_OFFSET_PER_ROW_CHROMATIC = 5;
    static const uint8_t NOTE_OFFSET_PER_ROW_INKEY     = 3;
};

inline
void Pads::PressedPadsBitfield::set(const fp::Vector2d<uint8_t>& coord, uint8_t vel) noexcept
{
    m_rows[coord.y][coord.x] = vel;
}

inline
void Pads::PressedPadsBitfield::clear(const fp::Vector2d<uint8_t>& coord) noexcept
{
    m_rows[coord.y][coord.x] = 0;
}

template <typename CbFunc>
void Pads::PressedPadsBitfield::forEach(CbFunc&& cbFunc) noexcept
{
    for(uint8_t _y = 0; _y < m_rows.size(); ++_y)
    {
        for(uint8_t _x = 0; _x < m_rows[_y].size(); ++_x)
        {
            if(m_rows[_y][_x])
            {
                cbFunc({_x, _y}, m_rows[_y][_x]);
            }
        }
    }
}



} // namespace push2

#endif