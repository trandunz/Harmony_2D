#version 460 core

out vec4 FragColor;

uniform float Time;

void main()
{
    FragColor = vec4(((sin(Time) / 2) + 0.5f),((sin(Time/2) / 2) + 0.5f),((sin(Time/4) / 2) + 0.5f),1.0f) * ((sin(Time) / 2) + 0.5f);
} 