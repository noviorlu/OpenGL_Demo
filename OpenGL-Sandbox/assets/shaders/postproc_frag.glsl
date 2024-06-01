#version 460 core
out vec4 FragColor;
in vec2 v_TexCoord; // varing TextCoord

uniform sampler2D u_ScreenTexture;

void main()
{
    FragColor = texture(u_ScreenTexture, v_TexCoord);
}