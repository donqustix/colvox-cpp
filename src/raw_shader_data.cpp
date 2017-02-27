#include "raw_shader_data.h"

using minecpp::RawShaderData;

RawShaderData::RawShaderData() noexcept
{
    ::glGenVertexArrays(1, &vao);
    ::glGenBuffers(1, &vbo);
}

RawShaderData::RawShaderData(RawShaderData&& rawShaderData) noexcept :
    vao{rawShaderData.vao}, vbo{rawShaderData.vbo}
{
    rawShaderData.vao = rawShaderData.vbo = 0;
}

RawShaderData::~RawShaderData()
{
    ::glDeleteVertexArrays(1, &vao);
    ::glDeleteBuffers(1, &vbo);
}

void RawShaderData::load() noexcept
{
    ::glBindVertexArray(vao);
    ::glBindBuffer(GL_ARRAY_BUFFER, vbo);
    fillBuffer();
    bindAttributePointers();
    ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    ::glBindVertexArray(0);
}

