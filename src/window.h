#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL_video.h>

#include <utility>
#include <string>

namespace minecpp
{
    class Window
    {
        SDL_Window* handle;
        int width;
        int height;

    public:
        friend void swap(Window& window1, Window& window2) noexcept
        {
            using std::swap;

            swap(window1.handle, window2.handle);
            swap(window1.width , window2.width );
            swap(window1.height, window2.height);
        }

        Window(const std::string& title, int width, int height,
                int x = SDL_WINDOWPOS_CENTERED,
                int y = SDL_WINDOWPOS_CENTERED, Uint32 flags = SDL_WINDOW_OPENGL);
        Window(Window&& window) noexcept;
        ~Window();

        Window& operator=(Window&& window) noexcept
        {
            if (this == &window) return *this;
            swap(*this, window);
            return *this;
        }

        const auto* getHandle() const noexcept {return handle;}
        auto getWidth() const noexcept {return width;}
        auto getHeight() const noexcept {return height;}
    };
}

#endif
