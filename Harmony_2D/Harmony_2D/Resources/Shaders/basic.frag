#version 460 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int ID;
layout (location = 2) out vec4 HitPosition;

in vec3 Position;
in mat4 Model_pass;
in mat4 View_pass;
in mat4 Proj_pass;

uniform float Time;
uniform int Id;

void main()
{
    FragColor = vec4(((sin(Time) / 2) + 0.5f),(1),(1),1.0f);
    ID = Id;
    HitPosition = Model_pass * vec4(Position,1.0f);
} 