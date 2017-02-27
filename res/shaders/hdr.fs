#version 450 core

layout (location = 0) uniform float power;

layout (binding = 0) uniform sampler2D colorTexture;
layout (binding = 1) uniform sampler2D blurTexture;

in vec2 fragTexCoord;

out vec3 finalColor;

const float exposure = 5.0;
const vec3 invGammaVec = vec3(1.0 / 2.2);

void main()
{
    const vec3 pixelColor = (texture(colorTexture, fragTexCoord) + texture(blurTexture,  fragTexCoord)).rgb;
    const vec3 mapped = vec3(1.0) - exp(-pixelColor * power);
    finalColor = pow(mapped, invGammaVec);
}

