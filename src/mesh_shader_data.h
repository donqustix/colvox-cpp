#ifndef MESHSHADERDATA_H
#define MESHSHADERDATA_H

#include "raw_shader_data.h"

#include <vector>

namespace minecpp
{
    struct VertexP3FRGBA8;
    struct VertexP2BUV8;

    template<typename T>
    class MeshShaderData : public RawShaderData
    {
    public:
        using VertexType = T;

    private:
        const std::vector<VertexType>* vertices;

        void fillBuffer() noexcept override
        {
            ::glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof (VertexType), vertices->data(), GL_STATIC_DRAW);
        }

        void bindAttributePointers() noexcept override;

    public:
        using RawShaderData::RawShaderData;
        using RawShaderData::operator=;

        MeshShaderData(const std::vector<VertexType>& data) noexcept :
            vertices{&data}
        {
            this->load();
        }
    };

    template<>
    inline void MeshShaderData<VertexP3FRGBA8>::bindAttributePointers() noexcept;

    template<>
    inline void MeshShaderData<VertexP2BUV8>::bindAttributePointers() noexcept;

    extern template class MeshShaderData<VertexP3FRGBA8>;
    extern template class MeshShaderData<VertexP2BUV8>;
}

#endif
