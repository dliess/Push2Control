#ifndef TAP_TEMPO_HANDLER_H
#define TAP_TEMPO_HANDLER_H

#include <chrono>

class TempoHandler;

class TapTempoHandler
{
public:
    TapTempoHandler(TempoHandler& rTempoHandler) noexcept;
    void tap() noexcept;
private:
    TempoHandler& m_rTempoHandler;
    std::chrono::system_clock::time_point m_lastTapTimepoint;
    std::chrono::nanoseconds m_sum;
    int                      m_count{0};
    static constexpr int NUM_PERIODS_TO_RESET_FROM{3};
    static constexpr int NUM_TAPS_TO_SET_BPM{2};
};

#endif