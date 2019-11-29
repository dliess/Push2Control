#ifndef KEY_VALUE_VECTOR_H
#define KEY_VALUE_VECTOR_H

#include <vector>

namespace util
{

template <typename Key, typename Value, typename Allocator = std::allocator<Value>>
class KeyValueVector
{
public:
    Value& operator[](const Key& key)
    {
        for(auto &e : m_vector)
        {
            if(e.first == key)
            {
                return e.second;
            }
        }
        m_vector.emplace_back(std::make_pair(key, Value()));
        return m_vector.back().second;
    }
    using iterator = typename std::vector< std::pair<Key, Value> >::iterator;
    using const_iterator = typename std::vector< std::pair<Key, Value> >::const_iterator;

    iterator find(const Key& key)
    {
        for(auto it = m_vector.begin(); it != m_vector.end(); ++it)
        {
            if(it->first == key)
            {
                return it;
            }
        }
        return m_vector.end();
    }
    const_iterator find(const Key& key) const
    {
        for(auto it = m_vector.begin(); it != m_vector.end(); ++it)
        {
            if(it->first == key)
            {
                return it;
            }
        }
        return m_vector.end();
    }
    iterator begin()
    {
      return m_vector.begin();
    }
    const_iterator begin() const
    {
      return m_vector.begin();
    }
    iterator end()
    {
      return m_vector.end();
    }
    const_iterator end() const
    {
      return m_vector.end();
    }

    size_t size() const noexcept {return m_vector.size();}
    std::pair<Key, Value> &atIdx(unsigned int idx) {return m_vector.at(idx);}
    const std::pair<Key, Value> &atIdx(unsigned int idx) const {return m_vector.at(idx);}

    std::pair<Key, Value> &back(){ return m_vector.back();}
    const std::pair<Key, Value> &back() const{ return m_vector.back();}

    int getIndexOf(const Key& key) const noexcept
    {
        for(int i = 0; i < size(); ++i)
        {
            if(atIdx(i).first == key) return i;
        }
        return -1;
    }

private:
    std::vector< std::pair<Key, Value> > m_vector;
};


} // namespace util
#endif
