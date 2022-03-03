#version 460 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 ID;

uniform float Time;

void main()
{
    FragColor = vec4(((sin(Time) / 2) + 0.5f),(1),(1),1.0f);
    ID = vec4(0.2f,0.2f,0.2f,1.0f);
} 