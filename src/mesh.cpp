#include "mesh.h"
#include "vertices.h"
#include "mesh_shader_data.h"

using minecpp::Mesh;

template<typename T>
Mesh<T>::Mesh() noexcept = default;

template<typename T>
Mesh<T>::Mesh(Mesh&&) noexcept = default;

template<typename T>
Mesh<T>::~Mesh() = default;

template<typename T>
Mesh<T>& Mesh<T>::operator=(Mesh&&) noexcept = default;

template<typename T>
void Mesh<T>::buffer(const std::vector<VertexType>& vertices) noexcept
{
    shaderData = std::make_unique<MeshShaderData<VertexType>>(vertices);
    vertexCount = vertices.size();
}

namespace minecpp
{
    template class Mesh<VertexP3FRGBA8>;
    template class Mesh<VertexP2BUV8>;
}

