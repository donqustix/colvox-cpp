#version 450 core

layout (location = 0) uniform mat4 matrix;

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertColor;
layout (location = 2) in vec4 vertLightColor;

out vec3 fragVertColor;
out vec4 fragVertLightColor;

void main()
{
    gl_Position = matrix * vec4(vertPos, 1.0);
    fragVertColor = vertColor;
    fragVertLightColor = vertLightColor;
}

