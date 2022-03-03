#version 460 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int ID;
layout (location = 2) out vec4 HitPosition;

in vec3 Position;
in mat4 MVP;

uniform float Time;
uniform int Id;

void main()
{
    FragColor = vec4(((sin(Time) / 2) + 0.5f),(1),(1),1.0f);
    ID = Id;
    HitPosition = vec4(Position.xyz + 0.5f,1.0f);
} 