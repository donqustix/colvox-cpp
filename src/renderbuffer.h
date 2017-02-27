#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H

#include <GL/glew.h>

#include <utility>

namespace minecpp
{
    class Renderbuffer
    {
        GLuint handle;

    public:
        friend void swap(Renderbuffer& renderbuffer1, Renderbuffer& renderbuffer2) noexcept
        {
            using std::swap;

            swap(renderbuffer1.handle, renderbuffer2.handle);
        }

        Renderbuffer(GLenum internalFormat, GLsizei width, GLsizei height) noexcept;
        Renderbuffer(Renderbuffer&& renderbuffer) noexcept;
        ~Renderbuffer();

        Renderbuffer& operator=(Renderbuffer&& renderbuffer) noexcept
        {
            if (this == &renderbuffer) return *this;
            swap(*this, renderbuffer);
            return *this;
        }

        auto getHandle() const noexcept {return handle;}
    };
}

#endif
