#ifndef UTIL_RINGBUFFER_H
#define UTIL_RINGBUFFER_H

#include <array>

namespace util
{

template<typename T, std::size_t N>
class Ringbuffer
{
public:
    static constexpr std::size_t Capacity = N;

    using value_type = T;

    void push(T v)
    {
        m_storage[wrap(m_head)] = std::move(v);
        m_head += 1;
        if(m_head - m_tail > Capacity)
        {
            m_tail = m_head - Capacity;
        }
    }

    value_type& front()
    {
        return m_storage[m_head - 1];
    }

    value_type& back()
    {
        return m_storage[m_tail];
    }

    std::size_t Size() const
    {
        return m_head - m_tail;
    }

    value_type& operator[](std::size_t idx)
    {
        return m_storage[wrap(m_tail + idx)];
    }

    const value_type& operator[](std::size_t idx) const
    {
        return m_storage[wrap(m_tail + idx)];
    }
private:
    std::array<T, N> m_storage;
    std::size_t      m_head{0};
    std::size_t      m_tail{0};

    static constexpr std::size_t wrap(std::size_t index)
    {
        return index % Capacity;
    }
};
} // namespace util

#endif