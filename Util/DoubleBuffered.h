#ifndef UTIL_DOUBLE_BUFFERED_H
#define UTIL_DOUBLE_BUFFERED_H

#include <atomic>
#include <functional>
#include <mutex>

namespace util
{

struct ClearInner
{
   template<typename T>
   void operator()(T& inner, T& outer)
   {
      inner.clear();
   }
};
struct ClearOuter
{
   template<typename T>
   void operator()(T& inner, T& outer)
   {
      outer.clear();
   }
};

template<typename T, typename AfterSwap = ClearOuter>
struct DoubleBuffered
{
   constexpr DoubleBuffered(const T& inner,
                           const T& outer) noexcept(std::is_nothrow_copy_constructible_v<T>)
      : m_store{{inner, outer}}
   {}

   constexpr DoubleBuffered(T&& inner, const T& outer) noexcept(
      std::is_nothrow_copy_constructible_v<T>&& std::is_nothrow_move_constructible_v<T>)
      : m_store{{std::move(inner), outer}}
   {}

   constexpr DoubleBuffered(const T& inner, T&& outer = T{}) noexcept(
      std::is_nothrow_copy_constructible_v<T>&& std::is_nothrow_move_constructible_v<T>)
      : m_store{{inner, std::move(outer)}}
   {}

   constexpr DoubleBuffered(T&& inner = T{},
                           T&& outer = T{}) noexcept(std::is_nothrow_move_constructible_v<T>)
      : m_store{{std::move(inner), std::move(outer)}}
   {}

   constexpr T& inner() noexcept
   {
      return m_store[m_innerIdx];
   }

   constexpr const T& inner() const noexcept
   {
      return m_store[m_innerIdx];
   }

   constexpr T& outer() noexcept
   {
      std::unique_lock lock(m_outerLock);
      return m_store[outerIdx()];
   }

   constexpr const T& outer() const noexcept
   {
      std::unique_lock lock(m_outerLock);
      return m_store[outerIdx()];
   }

   template<typename Func>
   constexpr decltype(auto) outerLocked(Func&& f)
   {
      std::unique_lock lock(m_outerLock);
      return std::invoke(std::forward<Func>(f), m_store[outerIdx()]);
   }

   template<typename Func>
   constexpr decltype(auto) outerLocked(Func&& f) const
   {
      std::unique_lock lock(m_outerLock);
      return std::invoke(std::forward<Func>(f), m_store[outerIdx()]);
   }

   constexpr void swap() noexcept
   {
      std::unique_lock lock(m_outerLock);
      m_innerIdx = outerIdx();
      AfterSwap{}(m_store[m_innerIdx % 2], m_store[outerIdx()]);
   }

private:
   std::array<T, 2>  m_store;
   std::atomic<int>  m_innerIdx{0};
   std::mutex        m_outerLock;
   const int outerIdx(){ return (m_innerIdx + 1) % 2; }
};

} // namespace util

#endif