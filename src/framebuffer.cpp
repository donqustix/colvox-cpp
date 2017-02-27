#include "framebuffer.h"

using minecpp::Framebuffer;

Framebuffer::Framebuffer() noexcept
{
    ::glGenFramebuffers(1, &handle);
}

Framebuffer::Framebuffer(Framebuffer&& framebuffer) noexcept : handle{framebuffer.handle}
{
    framebuffer.handle = 0;
}

Framebuffer::~Framebuffer()
{
    ::glDeleteFramebuffers(1, &handle);
}

void Framebuffer::bindTexture(GLenum attachment, GLuint texture) noexcept
{
    ::glBindFramebuffer(GL_FRAMEBUFFER, handle);
    ::glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
    ::glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bindRenderbuffer(GLenum attachment, GLuint renderbuffer) noexcept
{
    ::glBindFramebuffer(GL_FRAMEBUFFER, handle);
    ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer);
    ::glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

