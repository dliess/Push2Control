#ifndef THREADED_LOOP_H
#define THREADED_LOOP_H

#include <atomic>
#include <functional>
#include <thread>
#include <chrono>

namespace util
{
class ThreadedLoop
{
public:
   ThreadedLoop(){};
   template<class Function, class... Args>
   explicit ThreadedLoop(std::chrono::nanoseconds period, Function&& f,
                         Args&&... args)
   {
      m_thread = std::thread([this, period, f, args...]() {
         while (!m_terminateRequest)
         {
            auto start = std::chrono::high_resolution_clock::now();

            std::invoke(f, args...);

            auto end = std::chrono::high_resolution_clock::now();
            const std::chrono::nanoseconds diff = end - start;
            if (diff < period)
            {
               std::this_thread::sleep_for(period - diff);
            }
         }
      });
   }
   ThreadedLoop(const ThreadedLoop&) = delete;
   ThreadedLoop(ThreadedLoop&& rhs) noexcept :
      m_thread(std::move(rhs.m_thread)), m_terminateRequest()
   {
      m_terminateRequest.exchange(rhs.m_terminateRequest);
   }
   ThreadedLoop& operator=(ThreadedLoop&& rhs) noexcept
   {
      m_thread = std::move(rhs.m_thread);
      m_terminateRequest.exchange(rhs.m_terminateRequest);
      return *this;
   }
   ~ThreadedLoop()
   {
      m_terminateRequest.store(true);
      m_thread.join();
   }
   void join() { m_thread.join(); }

private:
   std::thread m_thread;
   std::atomic<bool> m_terminateRequest{false};
};

} // namespace util

#endif