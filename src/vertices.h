#ifndef VERTICES_H
#define VERTICES_H

#include <GL/glew.h>

namespace minecpp
{
    struct RGBA8
    {
        GLubyte r;
        GLubyte g;
        GLubyte b;
        GLubyte a;
    };

    struct RGB8
    {
        GLubyte r;
        GLubyte g;
        GLubyte b;
    };

    struct UV8
    {
        GLubyte u;
        GLubyte v;
    };

    struct VertexP3FRGBA8
    {
        struct Position
        {
            GLfloat x;
            GLfloat y;
            GLfloat z;
        } position;

        RGB8  rgb;
        RGBA8 rgba;
    };

    struct VertexP2BUV8
    {
        struct Position
        {
            GLbyte x;
            GLbyte y;
        } position;

        UV8 texCoord;
    };
}

#endif
