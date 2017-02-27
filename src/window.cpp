#include "window.h"

#include <stdexcept>

using minecpp::Window;

Window::Window(const std::string& title, int width, int height, int x, int y, Uint32 flags) :
    handle{::SDL_CreateWindow(title.c_str(), x, y, width, height, flags)}
{
    if (!handle)
        throw std::runtime_error{::SDL_GetError()};

    ::SDL_GetWindowSize(handle, &this->width, &this->height);
}

Window::Window(Window&& window) noexcept : handle{window.handle}, width{window.width}, height{window.height}
{
    window.handle = nullptr;
    window.width = window.height = 0;
}

Window::~Window()
{
    ::SDL_DestroyWindow(handle);
}

