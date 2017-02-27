#include "service.h"

#include <thread>

using minecpp::Service;

void Service::stopRunning() noexcept
{
    std::unique_lock<std::mutex> guard{mx};
    running = false;
    guard.unlock();
    
    cv.notify_one();
    guard.lock();

    cv.wait(guard, [this] {return done;});
}

void Service::start() noexcept
{
    std::thread t{[this] {

        while (running)
        {
            working();

            std::unique_lock<std::mutex> guard{mx};
            cv.wait(guard, [this] {return !waiting() || !running;});
        }

        std::unique_lock<std::mutex> guard{mx};
        done = true;
        std::notify_all_at_thread_exit(cv, std::move(guard));
    }};
    t.detach();
}

