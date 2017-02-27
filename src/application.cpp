#include "application.h"
#include "window.h"
#include "config.h"
#include "resource_container.h"
#include "resource_loader.h"
#include "timer.h"
#include "player.h"
#include "render_data.h"
#include "camera.h"
#include "chunk_manager.h"
#include "framebuffer.h"
#include "texture.h"
#include "shader_program.h"
#include "renderbuffer.h"
#include "textured_rect_mesh.h"
#include "mesh_shader_data.h"
#include "post_processing.h"

#include <SDL2/SDL.h>

#include <GL/glew.h>

#include <stdexcept>

namespace minecpp
{
    struct Application::SDL
    {
        SDL()
        {
            if (::SDL_Init(SDL_INIT_VIDEO) < 0)
                throw std::runtime_error{::SDL_GetError()};

            ::SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            ::SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            ::SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
            //::SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        }

        ~SDL()
        {
            ::SDL_Quit();
        }
    };

    class Application::GLContext
    {
        SDL_GLContext handle;

    public:
        explicit GLContext(const Window& window) :
            handle{::SDL_GL_CreateContext(const_cast<SDL_Window*>(window.getHandle()))}
        {
            if (!handle)
                throw std::runtime_error{::SDL_GetError()};

            ::glewExperimental = GL_TRUE;
            ::glewInit();
        }

        ~GLContext()
        {
            ::SDL_GL_DeleteContext(handle);
        }
    };
}

using minecpp::Application;

Application::Application() : sdl{std::make_unique<SDL>()}
{
    auto config = Config::parseFile("res/configs/application.cfg");

    window = std::make_unique<Window>("MineCPP", config.findValue<int>("w", "window"),
                                                 config.findValue<int>("h", "window"));
    if (config.findValue<bool>("fullscreen", "window"))
        ::SDL_SetWindowFullscreen(const_cast<SDL_Window*>(window->getHandle()), SDL_WINDOW_FULLSCREEN);

    glContext = std::make_unique<GLContext>(*window);

    resourceContainer = std::make_unique<ResourceContainer>();

    ResourceLoader{*resourceContainer}.load(Config::parseFile("res/configs/resources.cfg"));

    maxFps = config.findValue<unsigned>("max_fps");
}

Application::~Application() = default;

void Application::run()
{
    ChunkManager c{*resourceContainer};

    RenderData renderData{120.0F, static_cast<float>(window->getWidth()) / window->getHeight(), 1.0F, 5e4F};

    Player player{{}};

    Timer timer;

    const Texture colorTexture{GL_RGB16F, window->getWidth(), window->getHeight(), GL_RGB, GL_FLOAT, nullptr};
    const Renderbuffer renderbuffer{GL_DEPTH_COMPONENT, window->getWidth(), window->getHeight()};

    Framebuffer framebuffer;
    framebuffer.bindTexture(GL_COLOR_ATTACHMENT0, colorTexture.getHandle());
    framebuffer.bindRenderbuffer(GL_DEPTH_ATTACHMENT, renderbuffer.getHandle());

    const auto texturedRectShaderProgram = resourceContainer->get<ShaderProgram>("textured_rect_shader");

    const PostProcessing postProcessing{*resourceContainer, window->getWidth(), window->getHeight()};

    ::SDL_SetRelativeMouseMode(SDL_TRUE);
    ::glEnable(GL_DEPTH_TEST);

    const Uint32 spf = 1000 / maxFps;

    while (running)
    {
        const Uint32 elapsedTime = timer.calculateElapsedTime();

        ::SDL_PumpEvents();

        const Uint8* const keyboardState = ::SDL_GetKeyboardState(nullptr);

        if (keyboardState[SDL_SCANCODE_ESCAPE])
            running = false;
        
        static bool a = false;
        if (keyboardState[SDL_SCANCODE_F])
        {
            a = true;
        }
        else if (a)
        {
            a = false;
            const int r = 32;
            for (int i = 0; i < r; ++i)
            {
                for (int j = 0; j < r; ++j)
                {
                    for (int k = 0; k < r; ++k)
                    {
                        c.setVoxel(k - r / 2 + player.getCamera().getPosition().x / constants::BLOCK_SIZE,
                                   i - r / 2 + player.getCamera().getPosition().y / constants::BLOCK_SIZE,
                                   j - r / 2 + player.getCamera().getPosition().z / constants::BLOCK_SIZE, 0);
                    }
                }
            }
        }

        player.update(elapsedTime);
        renderData.setViewMatrix(player.getCamera().calculateViewMatrix());
        c.update();

        ::glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.getHandle());

        ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ::glClearColor(1.0F, 1.0F, 1.0F, 1.0F);

        c.render(renderData);

        ::glBindFramebuffer(GL_FRAMEBUFFER, 0);

        postProcessing.process(renderData, colorTexture);

        ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ::glUseProgram(texturedRectShaderProgram->getHandle());

        ::glActiveTexture(GL_TEXTURE0);
        ::glBindTexture(GL_TEXTURE_2D, postProcessing.getTexture().getHandle());

        ::glBindVertexArray(renderData.getTexturedRectMesh().getShaderData().getVao());
        ::glDrawArrays(GL_TRIANGLES, 0, renderData.getTexturedRectMesh().getVertexCount());
        ::glBindVertexArray(0);

        ::glBindTexture(GL_TEXTURE_2D, 0);

        ::glUseProgram(0);

        ::SDL_GL_SwapWindow(const_cast<SDL_Window*>(window->getHandle()));

        if (elapsedTime < spf)
            ::SDL_Delay(spf - elapsedTime);
    }
}

