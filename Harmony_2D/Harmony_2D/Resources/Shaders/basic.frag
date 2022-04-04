// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : basic.frag
// Description : Generic fragment shader for handling both animated meshes and non animated meshes
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core
#define PI 3.141592654

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec3 Position;
in vec2 TexCoords;

// Outside Variables Passed In As 'Uniforms'
uniform float Time;

uniform int TextureCount;
uniform int NumberOfAnimationFrames, CurrentAnimationFrame;
uniform bool IsAnimation;

uniform sampler2D Texture0, Texture1;

// Available Helper function Forward Declerations
float ClampedSin(float _value);
vec4 ColourFromTexturesORWhite(vec2 _texCoords);

// Main function that gets called per vertex fragment.
void main()
{
    if (IsAnimation  == true)
    {
        vec2 animatedFrameCoords = TexCoords;
        animatedFrameCoords.x = (CurrentAnimationFrame + TexCoords.x) * (1.0f/NumberOfAnimationFrames);
        FragColor = ColourFromTexturesORWhite(animatedFrameCoords);
    }
    else
    {
        FragColor = ColourFromTexturesORWhite(TexCoords);
    }
} 

// Clamps Sin Between 0 and 1
float ClampedSin(float _value)
{
    return ((sin(_value + (PI/2)))/2) + 0.5f;
}

// Checks for number of active textures and returns the colour output accordingly.
// If two textures are passed into the shader, they are mixed with a clamped sin function.
vec4 ColourFromTexturesORWhite(vec2 _texCoords)
{
    vec4 outputColour;
    switch(TextureCount)
    {
        case 1:
        {
            outputColour = texture(Texture0,_texCoords);
            break;
        }
        case 2:
        {
            outputColour = mix(texture(Texture0,_texCoords),texture(Texture1,_texCoords), ClampedSin(Time));
            break;
        }
        default:
        {
            outputColour = vec4(1.0f,1.0f,1.0f,1.0f);
            break;
        }
    }
    return outputColour;
}
