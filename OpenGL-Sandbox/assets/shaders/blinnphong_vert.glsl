#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main()
{
    vs_out.FragPos = aPos;
    vs_out.Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0);
}