#version 450 core

layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[2];

void main()
{
	if (v_TexIndex != -1.0) {
		o_Color = texture(u_Textures[int(v_TexIndex)], v_TexCoord);
	}
	else {
		o_Color = v_Color;
	}
}