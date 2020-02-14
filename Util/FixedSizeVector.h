#ifndef UTIL_FIXED_SIZE_VECTOR_H
#define UTIL_FIXED_SIZE_VECTOR_H

#include <array>

namespace util
{

template<typename T, std::size_t SIZE>
class FixedSizeVector
{
public:
   using value_type = T;
   using reference = value_type&;
   using const_reference = const value_type&;
   using size_type = std::size_t;
   using iterator = typename std::array<T, SIZE>::iterator;
   using const_iterator = typename std::array<T, SIZE>::const_iterator;

   // TODO noexcept constructors only when calue type constr. does not throw
   explicit FixedSizeVector() noexcept : m_storage(), m_endIter(m_storage.begin()){};
   FixedSizeVector(const FixedSizeVector&) noexcept = default;
   FixedSizeVector(FixedSizeVector&&) noexcept = default;
   /*
   // initializer list did not work:
   // https://stackoverflow.com/questions/6893700/how-to-construct-stdarray-object-with-initializer-list
   template<typename ...E>
   FixedSizeVector(E&&...e) : m_storage{{std::forward<E>(e)...}} {}
   */
   FixedSizeVector& operator=(const FixedSizeVector&) noexcept = default;
   FixedSizeVector& operator=(FixedSizeVector&&) noexcept = default;

   // Capacity
   size_type size() const noexcept { return std::distance(m_storage.begin(), static_cast<const_iterator>(m_endIter)); };

   // Iterators
   iterator begin() noexcept { return m_storage.begin(); }
   iterator end() noexcept { return m_endIter; }
   const_iterator begin() const noexcept { return m_storage.begin(); }
   const_iterator end() const noexcept { return m_endIter; }

   // Mutators
   void push_back(const value_type& val)
   {
      if(m_storage.end() == m_endIter)
      {
         throw std::length_error("");
      }
      m_storage[size()] = val;
      m_endIter++;
   }
   void erase(iterator position)
   {
      iterator iter = position;
      for(; iter < m_endIter; iter++)
      {
         if(iter+1 < m_endIter)
         {
            *iter = *(iter+1);
         }
      }
      if(begin() != m_endIter) m_endIter--;
   }

   // Selectors
   reference operator[](int idx) { return m_storage[idx]; }
   const_reference operator[](int idx) const{ return m_storage[idx]; }

private:
   using Storage = std::array<T, SIZE>;
   Storage  m_storage;
   iterator m_endIter;
};

} // namespace util

#endif