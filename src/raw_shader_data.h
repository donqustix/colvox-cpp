#ifndef RAWSHADERDATA_H
#define RAWSHADERDATA_H

#include <utility>

#include <GL/glew.h>

namespace minecpp
{
    class RawShaderData
    {
        GLuint vao;
        GLuint vbo;

    protected:
        virtual void fillBuffer() noexcept = 0;
        virtual void bindAttributePointers() noexcept = 0;

    public:
        friend void swap(RawShaderData& rawShaderData1, RawShaderData& rawShaderData2) noexcept
        {
            using std::swap;

            swap(rawShaderData1.vao, rawShaderData2.vao);
            swap(rawShaderData1.vbo, rawShaderData2.vbo);
        }

        RawShaderData() noexcept;
        RawShaderData(RawShaderData&& rawShaderData) noexcept;
        virtual ~RawShaderData();

        RawShaderData& operator=(RawShaderData&& rawShaderData) noexcept
        {
            if (this == &rawShaderData) return *this;
            swap(*this, rawShaderData);
            return *this;
        }

        void load() noexcept;

        auto getVao() const noexcept {return vao;}
    };
}

#endif
