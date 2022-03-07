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

vec4 WorldPosFromDepth(float _depth);
vec3 reconstruct_pos();

void main()
{
    FragColor = texture(Diffuse,TexCoords) * vec4(((sin(Time) / 2) + 0.5f),(1),(1),1.0f);
    ID = Id;
    HitPosition = vec4(reconstruct_pos(),1.0f);
} 

vec3 reconstruct_pos()
{
    vec2 screen;
    screen.x = ( gl_FragCoord.x - 1080 / 2.0 ) / ( 1080 / 2.0 );
    screen.y = ( (1080 - gl_FragCoord.y) - 1080 / 2.0 ) / ( -1080 / 2.0 );
    float D = Depth * 2.0 - 1.0; 
    vec4 WorldPos = inverse(Proj_pass * View_pass) * vec4( screen.x, screen.y, D, 1.0);
    WorldPos.xyz /= WorldPos.w;
    return WorldPos.xyz;
}
