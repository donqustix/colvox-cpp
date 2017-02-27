#include "renderbuffer.h"

using minecpp::Renderbuffer;

Renderbuffer::Renderbuffer(GLenum internalFormat, GLsizei width, GLsizei height) noexcept
{
    ::glGenRenderbuffers(1, &handle);
    ::glBindRenderbuffer(GL_RENDERBUFFER, handle);
    ::glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    ::glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Renderbuffer::Renderbuffer(Renderbuffer&& renderbuffer) noexcept : handle{renderbuffer.handle}
{
    renderbuffer.handle = 0;
}

Renderbuffer::~Renderbuffer()
{
    ::glDeleteRenderbuffers(1, &handle);
}

