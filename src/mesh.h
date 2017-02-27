#ifndef MESH_H
#define MESH_H

#include <memory>
#include <vector>

namespace minecpp
{
    struct VertexP3FRGBA8;
    struct VertexP2BUV8;

    template<typename T>
    class MeshShaderData;

    template<typename T>
    class Mesh
    {
    public:
        using VertexType = T;

    private:
        std::unique_ptr<MeshShaderData<VertexType>> shaderData;

        typename std::vector<VertexType>::size_type vertexCount;

    public:
        Mesh() noexcept;
        Mesh(Mesh&&) noexcept;
        ~Mesh();

        Mesh& operator=(Mesh&&) noexcept;

        void buffer(const std::vector<VertexType>& vertices) noexcept;

        const auto& getShaderData() const noexcept {return *shaderData;}
        auto getVertexCount() const noexcept {return vertexCount;}
    };

    extern template class Mesh<VertexP3FRGBA8>;
    extern template class Mesh<VertexP2BUV8>;
}

#endif
