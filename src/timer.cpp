#include "timer.h"

#include <SDL2/SDL_timer.h>

using minecpp::Timer;

Uint32 Timer::calculateElapsedTime() noexcept
{
    const Uint32 currentTime = ::SDL_GetTicks();
    const Uint32 elapsedTime = currentTime - previousTime;
    previousTime = currentTime;

    return elapsedTime;
}

