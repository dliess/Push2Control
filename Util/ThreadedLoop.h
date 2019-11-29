#ifndef THREADED_LOOP_H
#define THREADED_LOOP_H

#include <thread>
#include <atomic>
#include <functional>
#include <loguru.hpp>

namespace util
{

class ThreadedLoop
{
public:
    ThreadedLoop() {};
    template< class Function, class... Args > 
    explicit ThreadedLoop(Function&& f, Args&&... args) noexcept
    {
        try{
            m_thread = std::thread([this, f, args...](){while(!m_terminateRequest) std::invoke(f, args...);});
        }catch(std::exception& e)
        {
            LOG_F(ERROR, "ThreadedLoop() creation: {}", e.what());
        }
    }
    ThreadedLoop(const ThreadedLoop&) = delete;
    ThreadedLoop(ThreadedLoop&& rhs) noexcept:
        m_thread(std::move(rhs.m_thread)),
        m_terminateRequest()
    {
        m_terminateRequest.exchange(rhs.m_terminateRequest);
    }
    ThreadedLoop& operator=(ThreadedLoop&& rhs) noexcept
    {
        m_thread = std::move(rhs.m_thread);
        m_terminateRequest.exchange(rhs.m_terminateRequest);
        return *this;
    }
    ~ThreadedLoop() noexcept
    {
        LOG_F(INFO, "Entering ~ThreadedLoop()");
        m_terminateRequest.store(true);
        try{
            m_thread.join();
        }
        catch(std::exception& e)
        {
            LOG_F(ERROR, "~ThreadedLoop() at Join(): {}", e.what());
        };
        LOG_F(INFO, "Leaving ~ThreadedLoop()");
    }
private:
    std::thread       m_thread;
    std::atomic<bool> m_terminateRequest{false};
};

} // namespace utils

#endif