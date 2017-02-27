#include "texture.h"

using minecpp::Texture;

Texture::Texture(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type,
        const GLvoid* data) noexcept
{
    ::glGenTextures(1, &handle);
    ::glBindTexture(GL_TEXTURE_2D, handle);
    ::glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    ::glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture&& texture) noexcept : handle{texture.handle}
{
    texture.handle = 0;
}

Texture::~Texture()
{
    ::glDeleteTextures(1, &handle);
}

