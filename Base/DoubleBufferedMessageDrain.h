#ifndef DOUBLE_BUFFERED_MESSAGE_DRAIN_H
#define DOUBLE_BUFFERED_MESSAGE_DRAIN_H

#include "MidiMessage.h"
#include "DoubleBuffered.h"
#include "MidiInputCbBase.h"

#include <vector>

namespace midi
{

class DoubleBufferedMessageDrain : public MidiInputCbBase
{
public:
    inline void update();
protected:
    inline void toBuffer(MidiMessage msg);

private:
    util::DoubleBuffered<std::vector<MidiMessage>> m_msgBuf;
};

} // namespace midi

inline
void midi::DoubleBufferedMessageDrain::update()
{
    m_msgBuf.swap();
    if(m_msgBuf.inner().size())
    {
        for(auto& msg : m_msgBuf.inner())
        {
            MidiInputCbBase::fromBuffer(msg);
        }
        return;
    }
}

inline
void midi::DoubleBufferedMessageDrain::toBuffer(MidiMessage msg)
{
    m_msgBuf.outerLocked( [&](auto& buf){
        buf.emplace_back(std::move(msg));
    } );
}

#endif