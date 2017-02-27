#version 450 core

layout (binding = 0) uniform sampler2D colorTexture;

in vec2 fragTexCoord;

out vec3 finalColor;

void main()
{
    const vec3 pixelColor = texture(colorTexture, fragTexCoord).rgb;
    finalColor = length(pixelColor) > 0.2 ? pixelColor : vec3(0.0);
}

