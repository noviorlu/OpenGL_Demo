#version 460 core
layout(location = 0) out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

in vec2 v_TexCoord; // varing TextCoord

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
}