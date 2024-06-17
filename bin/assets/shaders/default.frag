
#version 400 core

const int NUM_CASCADES = 4;
out vec4 FragColor;



in  vec3 FragPos;
in  vec2 TexCoords;
in  vec3 Normal; 

in vec3 outViewPosition;
in vec4 outWorldPosition;

struct CascadeShadow 
{
    mat4 projViewMatrix;
    float splitDistance;
};





uniform sampler2D diffuseTexture;
uniform CascadeShadow cascadeshadows[NUM_CASCADES];
uniform sampler2D shadowMap[NUM_CASCADES];





uniform vec3  lightDir;
uniform float farPlane;
uniform vec3  viewPos;

    const float shininess = 32.0;




    float CalculateShadow(vec4 worldPosition, int idx) 
    {
        vec2 texelSize  = 1.0 / textureSize(shadowMap[idx], 0);
        vec4 shadowMapPosition = cascadeshadows[idx].projViewMatrix * worldPosition;
        vec4 projCoords = (shadowMapPosition / shadowMapPosition.w) * 0.5 + 0.5;

        float currentDepth = projCoords.z;

        // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
        if (currentDepth > 1.0)
        {
            return 0.0;
        }

        vec3 normal = normalize(Normal);
        float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
        const float biasModifier = 0.5f;

        if (idx == NUM_CASCADES)
        {
            bias *= 1 / (farPlane * biasModifier);
        }
        else
        {
            bias *= 1 / (cascadeshadows[idx].splitDistance * biasModifier);
        }

        float shadow = 0.0;
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                vec2 texel = projCoords.xy + vec2(x, y) * texelSize;
                float pcfDepth = texture(shadowMap[idx], texel).r;
                shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        shadow /= 9.0;

        return shadow;
    }






void main()
{           
    vec3 color = texture(diffuseTexture, TexCoords).rgb;
    vec3 viewDir = normalize(viewPos - FragPos);
    
    
    int cascadeIndex = 0;
    for (int i=0; i<NUM_CASCADES - 1; i++) 
    {
        if (outViewPosition.z < cascadeshadows[i].splitDistance) 
        {
            cascadeIndex = i + 1;
            }
    }


    

    //luz para shadow "sol"
    vec3 normal = normalize(Normal);
    vec3 lightColor = vec3(0.6);
    vec3 ambient = 0.1 * color;
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = spec * lightColor;    

    float shadow = CalculateShadow(outWorldPosition, cascadeIndex);                      
    



        
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    

    

    
    FragColor = vec4(lighting, 1.0);
}