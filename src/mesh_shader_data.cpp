#include "mesh_shader_data.h"
#include "vertices.h"

using minecpp::MeshShaderData;

using minecpp::VertexP3FRGBA8;
using minecpp::VertexP2BUV8;

template<>
void MeshShaderData<VertexP3FRGBA8>::bindAttributePointers() noexcept
{
    ::glVertexAttribPointer(0, 3, GL_FLOAT        , GL_FALSE, sizeof (VertexP3FRGBA8), nullptr);
    ::glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE , sizeof (VertexP3FRGBA8),
            reinterpret_cast<GLvoid*>(sizeof VertexP3FRGBA8::position));
    ::glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE , sizeof (VertexP3FRGBA8),
            reinterpret_cast<GLvoid*>(sizeof VertexP3FRGBA8::position + sizeof VertexP3FRGBA8::rgb));
    ::glEnableVertexAttribArray(0);
    ::glEnableVertexAttribArray(1);
    ::glEnableVertexAttribArray(2);
}

template<>
void MeshShaderData<VertexP2BUV8>::bindAttributePointers() noexcept
{
    ::glVertexAttribPointer(0, 2, GL_BYTE         , GL_TRUE, sizeof (VertexP2BUV8), nullptr);
    ::glVertexAttribPointer(1, 2, GL_UNSIGNED_BYTE, GL_TRUE, sizeof (VertexP2BUV8),
            reinterpret_cast<GLvoid*>(sizeof VertexP2BUV8::position));
    ::glEnableVertexAttribArray(0);
    ::glEnableVertexAttribArray(1);
}

namespace minecpp
{
    template class MeshShaderData<VertexP3FRGBA8>;
    template class MeshShaderData<VertexP2BUV8>;
}

