#ifndef UTIL_LOCKED_H
#define UTIL_LOCKED_H

#include <functional>
#include <mutex>

namespace util
{

template<typename T>
class Locked
{
public:
   template<typename... Args>
   Locked(Args&&... args) : m_contents{std::forward<Args>(args)...}
   {}

   std::mutex mutex;

   template<typename Func>
   auto apply(Func&& f)
   {
      auto lock = std::unique_lock(mutex);
      return std::invoke(std::forward<Func>(f), m_contents);
   }

   template<typename Func>
   auto apply(Func&& f) const
   {
      auto lock = std::unique_lock(mutex);
      return std::invoke(std::forward<Func>(f), m_contents);
   }

   T& unsafeAccess()
   {
      return m_contents;
   }

   const T& unsafeAccess() const
   {
      return m_contents;
   }
private:
   T m_contents;
};

} // namespace util

#endif