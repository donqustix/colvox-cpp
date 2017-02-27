#version 450 core

in vec3 fragVertColor;
in vec4 fragVertLightColor;

out vec3 finalColor;

const float correct = 17.0;

void main()
{
    const vec4 lightColorCorrect = fragVertLightColor * correct;
    finalColor = fragVertColor * correct * pow(pow(lightColorCorrect.rgb, vec3(1.5)) + 
                                               pow(lightColorCorrect.a,        1.5), vec3(2.0));
}

