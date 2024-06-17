#version 400 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;

layout(location = 5) in ivec4 aBones;
layout(location = 6) in vec4 aWeights;


out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

out vec3 outViewPosition;
out vec4 outWorldPosition;


uniform vec3 viewPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform int isStatic; 

const int MAX_BONES = 80;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 Joints[MAX_BONES];




void main() 
{

    if (isStatic==1)
    {
        vec4 initPos = vec4(aPos, 1.0);

        FragPos = vec3(model * initPos);
        Normal = transpose(inverse(mat3(model))) * aNormal;

        mat4 modelViewMatrix = view * model;
        vec4 mvPosition =  modelViewMatrix * initPos;
        outViewPosition  = mvPosition.xyz;
        outWorldPosition = model * initPos;                
        
        gl_Position = projection * view * model * initPos;


    } else 
    {
    vec4 totalPosition = vec4(0.0f);
        vec4 totalNormal = vec4(0.0f);
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
        vec4 localNormal = Joints[aBones[i]] * vec4(aNormal, 0.0f); 
        totalNormal += localNormal * aWeights[i];
    }

        mat4 viewModel = view * model;
        
        Normal = mat3(transpose(inverse(viewModel))) * totalNormal.xyz; 

        FragPos = (model * totalPosition).xyz; 

    

        
        mat4 modelViewMatrix = view * model;
        vec4 mvPosition =  modelViewMatrix * totalPosition;
        outViewPosition  = mvPosition.xyz;
        outWorldPosition = model * totalPosition;    

        gl_Position = projection * viewModel * totalPosition;    

    
    }

    TexCoords = aTexCoords;
}