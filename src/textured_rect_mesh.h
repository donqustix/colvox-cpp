#ifndef TEXTUREDRECTMESH_H
#define TEXTUREDRECTMESH_H

#include "mesh.h"

namespace minecpp
{
    class TexturedRectMesh : public Mesh<VertexP2BUV8>
    {
    public:
        TexturedRectMesh() noexcept;
    };
}

#endif
