#include "shader_program.h"

#include <stdexcept>

using minecpp::ShaderProgram;

ShaderProgram::ShaderProgram() noexcept : handle{::glCreateProgram()} {}

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept :
    shaders{std::move(shaderProgram.shaders)}, handle{shaderProgram.handle}
{
    shaderProgram.handle = 0;
}

ShaderProgram::~ShaderProgram()
{
    deleteShaders();
    ::glDeleteProgram(handle);
}

void ShaderProgram::uniform(GLint location, const GLfloat* matrix, GLboolean transpose) noexcept
{
    ::glUniformMatrix4fv(location, 1, transpose, matrix);
}

void ShaderProgram::uniform(GLint location, GLint value) noexcept
{
    ::glUniform1i(location, value);
}

void ShaderProgram::loadShader(const std::string& source, GLenum type)
{
    const GLuint shader = ::glCreateShader(type);
    const auto raw_str = source.c_str();
    ::glShaderSource(shader, 1, &raw_str, nullptr);
    ::glCompileShader(shader);
    GLint compileResult;
    ::glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE)
    {
        GLint bufferLength;
        ::glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufferLength);
        std::vector<GLchar> buffer(bufferLength);
        ::glGetShaderInfoLog(shader, bufferLength, nullptr, buffer.data());
        ::glDeleteShader(shader);
        throw std::runtime_error{std::string{buffer.cbegin(), buffer.cend()}};
    }
    shaders.push_back(shader);
    ::glAttachShader(handle, shader);
}

void ShaderProgram::link()
{
    ::glLinkProgram(handle);
    deleteShaders();
    GLint linkResult;
    ::glGetProgramiv(handle, GL_LINK_STATUS, &linkResult);
    if (linkResult == GL_FALSE)
    {
        GLint bufferLength;
        ::glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &bufferLength);
        std::vector<GLchar> buffer(bufferLength);
        ::glGetProgramInfoLog(handle, bufferLength, nullptr, buffer.data());
        throw std::runtime_error{std::string{buffer.cbegin(), buffer.cend()}};
    }
}

void ShaderProgram::deleteShaders() noexcept
{
    for (auto s : shaders)
    {
        ::glDetachShader(handle, s);
        ::glDeleteShader(s);
    }
    shaders.clear();
    shaders.shrink_to_fit();
}

