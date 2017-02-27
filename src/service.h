#ifndef SERVICE_H
#define SERVICE_H

#include <condition_variable>
#include <mutex>

namespace minecpp
{
    class Service
    {
    protected:
        std::condition_variable cv;
        std::mutex mx;

    private:
        bool running = true;
        bool done = false;

    protected:
        virtual void working() noexcept = 0;
        virtual bool waiting() const noexcept = 0;

        void stopRunning() noexcept;

    public:
        virtual ~Service() = default;

        void start() noexcept;
        void doWork() noexcept {cv.notify_one();}
    };
}

#endif
