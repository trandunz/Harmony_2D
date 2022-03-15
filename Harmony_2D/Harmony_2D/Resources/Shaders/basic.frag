#version 460 core
#define PI 3.141592654

layout (location = 0) out vec4 FragColor;

in vec3 Position;
in vec2 TexCoords;

uniform float Time;
uniform int NumberOfAnimationFrames;
uniform int CurrentAnimationFrame;
uniform int IsAnimation;
uniform sampler2D Texture0;
uniform sampler2D Texture1;

vec3 GrabPositionFromDepth();

void main()
{
    vec2 animatedFrameCoords;
    if (IsAnimation  == 1)
    {
        animatedFrameCoords.x = (CurrentAnimationFrame + TexCoords.x) * (1.0f/NumberOfAnimationFrames);
    }
    else
    {
        animatedFrameCoords.x = TexCoords.x; 
    }
    animatedFrameCoords.y = TexCoords.y; 
    FragColor = mix(texture(Texture0,animatedFrameCoords) ,texture(Texture1,animatedFrameCoords), ((sin(Time + (PI/2)))/2)+0.5f);
} 
