#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include <utility>

namespace minecpp
{
    class Texture
    {
        GLuint handle;

    public:
        friend void swap(Texture& texture1, Texture& texture2) noexcept
        {
            using std::swap;

            swap(texture1.handle, texture2.handle);
        }

        Texture(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type,
                const GLvoid* data) noexcept;
        Texture(Texture&& texture) noexcept;
        ~Texture();

        Texture& operator=(Texture&& texture) noexcept
        {
            if (this == &texture) return *this;
            swap(*this, texture);
            return *this;
        }

        auto getHandle() const noexcept {return handle;}
    };
}

#endif
