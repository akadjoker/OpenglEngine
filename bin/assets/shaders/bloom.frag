
#version 400 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureMap;


    //Bloom
const float Samples = 8.0;          // Pixels per axis; higher = bigger glow, worse performance
const float Quality = 3.0;          // Defines size factor: Lower = smaller glow, better quality


//blur
float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);

vec4 Bloom(float samples, float quality);
vec4 Blur();

void main()
{
    
    //FragColor = texture(textureMap, TexCoords);
    FragColor = Bloom(Samples, Quality);
}
vec4 Bloom(float samples, float quality)
{
    vec2 size  = textureSize(textureMap, 0);
    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1)/size*quality;
    vec4 colDiffuse = vec4(1,1,1,1);
    


    vec4 source = texture(textureMap, TexCoords);

    const int range = 2;            // should be = (samples - 1)/2;

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            sum += texture(textureMap, TexCoords + vec2(x, y)*sizeFactor);
        }
    }
    
    vec4 color = ((sum/(samples*samples)) + source)*colDiffuse;
    return color;
}
vec4 Blur()
{
        vec3 texelColor = texture(textureMap, TexCoords).rgb*weight[0];
        vec2 size  = textureSize(textureMap, 0);

    for (int i = 1; i < 3; i++)
    {
        texelColor += texture(textureMap, TexCoords + vec2(offset[i])/size.x, 0.0).rgb*weight[i];
        texelColor += texture(textureMap, TexCoords - vec2(offset[i])/size.x, 0.0).rgb*weight[i];
    }

    return   vec4(texelColor, 1.0);

}