#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>

namespace minecpp
{
    class Window;
    class ResourceContainer;

    class Application
    {
        struct SDL;
        std::unique_ptr<SDL> sdl;

        std::unique_ptr<Window> window;

        class GLContext;
        std::unique_ptr<GLContext> glContext;

        std::unique_ptr<ResourceContainer> resourceContainer;

        unsigned maxFps;
        bool running = true;

    public:
        Application();
        Application(const Application&) = delete;
        ~Application();

        Application& operator=(const Application&) = delete;

        void run();
    };
}

#endif
