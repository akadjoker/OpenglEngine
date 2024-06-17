#version 400 core
layout (location = 0) in vec3 aPos;
layout(location = 5) in ivec4 aBones;
layout(location = 6) in vec4 aWeights;


uniform mat4 model;
uniform int isStatic;

const int MAX_BONES = 80;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 Joints[MAX_BONES];

uniform mat4 lightSpaceMatrix;


void main()
{
if (isStatic==1)
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
else 
{
    vec4 totalPosition = vec4(0.0f);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (aBones[i] == -1)
            continue;
        if (aBones[i] >= MAX_BONES)
        {
            totalPosition = vec4(aPos, 1.0f);
            break;
        }
        vec4 localPosition = Joints[aBones[i]] * vec4(aPos, 1.0f);
        totalPosition += localPosition * aWeights[i];
    }

    gl_Position =  lightSpaceMatrix*model * totalPosition;   
}
}