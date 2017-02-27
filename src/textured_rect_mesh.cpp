#include "textured_rect_mesh.h"
#include "vertices.h"

using minecpp::TexturedRectMesh;

TexturedRectMesh::TexturedRectMesh() noexcept
{
    this->buffer({{{-128, -128}, {  0,   0}},
                  {{ 127, -128}, {255,   0}},
                  {{ 127,  127}, {255, 255}},
                  {{ 127,  127}, {255, 255}},
                  {{-128,  127}, {  0, 255}},
                  {{-128, -128}, {  0,   0}}});
}

