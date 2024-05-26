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

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    FragColor = vec4(vec3(depth), 1.0);
}