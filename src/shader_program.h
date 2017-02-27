#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "resource.h"

#include <GL/glew.h>

#include <utility>
#include <vector>
#include <string>

namespace minecpp
{
    class ShaderProgram : public Resource
    {
        std::vector<GLuint> shaders;
        GLuint handle;

        void deleteShaders() noexcept;

    public:
        friend void swap(ShaderProgram& shaderProgram1, ShaderProgram& shaderProgram2) noexcept
        {
            using std::swap;

            swap(shaderProgram1.shaders, shaderProgram2.shaders);
            swap(shaderProgram1.handle , shaderProgram2.handle );
        }

        ShaderProgram() noexcept;
        ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
        ~ShaderProgram();

        ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept
        {
            if (this == &shaderProgram) return *this;
            swap(*this, shaderProgram);
            return *this;
        }

        void uniform(GLint location, const GLfloat* matrix, GLboolean transpose = GL_FALSE) noexcept;
        void uniform(GLint location, GLint value) noexcept;

        void loadShader(const std::string& source, GLenum type);
        void link();

        auto getHandle() const noexcept {return handle;}
    };
}

#endif
