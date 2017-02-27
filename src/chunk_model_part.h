#ifndef CHUNKMODELPART_H
#define CHUNKMODELPART_H

//#include "mesh.h"
//#include "shader_program.h"

#include <memory>

namespace minecpp
{
    template<typename T>
    class Mesh;

    class ShaderProgram;

    struct VertexP3FRGBA8;

    struct ChunkModelPart
    {
        std::unique_ptr<Mesh<VertexP3FRGBA8>> mesh;
        ShaderProgram* shaderProgram;
    };
}

#endif
