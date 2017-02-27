#version 450 core

layout (binding = 0) uniform sampler2D colorTexture;

in vec2 fragTexCoord;

out vec3 finalColor;

void main()
{
    finalColor = texture(colorTexture, fragTexCoord).rgb;
}

