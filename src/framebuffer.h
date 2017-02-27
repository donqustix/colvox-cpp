#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>

#include <utility>

namespace minecpp
{
    class Framebuffer
    {
        GLuint handle;

    public:
        friend void swap(Framebuffer& framebuffer1, Framebuffer& framebuffer2) noexcept
        {
            using std::swap;

            swap(framebuffer1.handle, framebuffer2.handle);
        }

        Framebuffer() noexcept;
        Framebuffer(Framebuffer&& framebuffer) noexcept;
        ~Framebuffer();

        Framebuffer& operator=(Framebuffer&& framebuffer) noexcept
        {
            if (this == &framebuffer) return *this;
            swap(*this, framebuffer);
            return *this;
        }

        void bindTexture(GLenum attachment, GLuint texture) noexcept;
        void bindRenderbuffer(GLenum attachment, GLuint renderbuffer) noexcept;

        auto getHandle() const noexcept {return handle;}
    };
}

#endif
