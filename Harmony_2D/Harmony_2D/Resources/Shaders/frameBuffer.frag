#version 460 core

in vec3 Position;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

const float offset_x = 1.0f/1080.0f;
const float offset_y = 1.0f/1080.0f;

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x, offset_y), vec2(0.0f, offset_y),vec2(offset_x, offset_y),
    vec2(-offset_x, 0.0f), vec2(0.0f, 0.0f),vec2(offset_x, 0.0f),
    vec2(-offset_x, -offset_y), vec2(0.0f, -offset_y),vec2(offset_x, -offset_y)
);

float kernel[9] = float[]
(  
    0,0,0,
    0,1,0,
    0,0,0
);

void main()
{
    vec3 color = vec3(0.0f);
    for (int i = 0; i < 9; i++)
        color += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];
    FragColor = vec4(color,1.0f);
} 