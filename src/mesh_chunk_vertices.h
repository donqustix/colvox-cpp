#ifndef MESHCHUNKVERTICES_H
#define MESHCHUNKVERTICES_H

#include "vertices.h"

#include <vector>

namespace minecpp
{
    struct MeshChunkVertices
    {
        std::vector<VertexP3FRGBA8> v_default;
    };
}

#endif
