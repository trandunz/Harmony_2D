#version 460 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int ID;
layout (location = 2) out vec4 HitPosition;

in vec3 Position;
in vec2 TexCoords;
in mat4 Model_pass;
in mat4 View_pass;
in mat4 Proj_pass;

uniform float Time;
uniform int Id;
uniform float Depth;
uniform sampler2D Diffuse;

vec3 GrabPositionFromDepth();

void main()
{
    FragColor = texture(Diffuse,TexCoords);
    ID = Id;
    HitPosition = Model_pass * vec4(Position,1.0f);
} 
