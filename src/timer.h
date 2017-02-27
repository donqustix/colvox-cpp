#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL_stdinc.h>

namespace minecpp
{
    class Timer
    {
        Uint32 previousTime;

    public:
        Uint32 calculateElapsedTime() noexcept;
    };
}

#endif
