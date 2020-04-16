#ifndef MIDI_UNIQUE_CC_MSG_VECTOR_H
#define MIDI_UNIQUE_CC_MSG_VECTOR_H

#include "MidiMessage.h"
#include <vector>

namespace midi
{

class UniqueCCMsgVector
{
public:
    using iterator = typename std::vector<Message<ControlChange>>::iterator;
    using const_iterator = typename std::vector<Message<ControlChange>>::const_iterator;

    void push_back(const Message<ControlChange>& ccMsgNew) noexcept
    {
        for(auto &ccMsg : m_vector)
        {
            if(ccMsg.channel() == ccMsgNew.channel() && 
               ccMsg.controllerNumber() == ccMsgNew.controllerNumber())
            {
                ccMsg = ccMsgNew;
                return;
            }
        }
        m_vector.push_back(ccMsgNew);
    }
    size_t size() const noexcept { return m_vector.size(); }
    iterator begin() noexcept
    {
      return m_vector.begin();
    }
    const_iterator begin() const noexcept
    {
      return m_vector.begin();
    }
    iterator end() noexcept
    {
      return m_vector.end();
    }
    const_iterator end() const noexcept
    {
      return m_vector.end();
    }
    void clear() noexcept { m_vector.clear(); }
private:
    std::vector<Message<ControlChange>> m_vector;
};

} // namespace midi

#endif