#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    vec3 emissiveFactor;
    int doubleSided;
    vec4 baseColorFactor;
    float metallicFactor;
    float roughnessFactor;
    sampler2D baseColorTexture;
};

uniform Material material;

void main()
{
    if (material.baseColorFactor.x == -1.0) {
        FragColor = texture(material.baseColorTexture, TexCoords);
    }
    else 
		FragColor = material.baseColorFactor;
}