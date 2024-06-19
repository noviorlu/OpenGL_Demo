#version 460 core
out vec4 FragColor;
in vec2 v_TexCoord; // varing TextCoord

uniform sampler2D u_ScreenTexture;

// Strictly this should be calculated using the screen resolution
const float offset = 1.0 / 300;

void main()
{
    // Inverse Color
    //FragColor = vec4(vec3(1.0 - texture(u_ScreenTexture, v_TexCoord)), 1.0);

    // Grayscale
    //FragColor = texture(u_ScreenTexture, v_TexCoord);
    //float average = 0.2126f * FragColor.r + 0.7152f * FragColor.g + 0.0722f * FragColor.b;
    //FragColor = vec4(average, average, average, 1.0f);

    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f, offset),  // top-center
        vec2(offset, offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f, 0.0f),    // center-center
        vec2(offset, 0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f, -offset), // bottom-center
        vec2(offset, -offset)  // bottom-right
    );
    float kernel[9] = float[](
        0, -1, 0,
        -1, 5, -1,
        0, -1, 0
    );
    /*float kernel[9] = float[](
        0.11111111, 0.11111111, 0.11111111,
        0.11111111, 0.11111111, 0.11111111,
        0.11111111, 0.11111111, 0.11111111
    ); */
    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_ScreenTexture, v_TexCoord.st + offsets[i]));
    }
    vec3 col;
    for (int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}
