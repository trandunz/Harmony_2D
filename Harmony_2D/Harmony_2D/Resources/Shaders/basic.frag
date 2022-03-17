#version 460 core
#define PI 3.141592654

layout (location = 0) out vec4 FragColor;

in vec3 Position;
in vec2 TexCoords;

uniform float Time;

uniform int TextureCount;
uniform int NumberOfAnimationFrames, CurrentAnimationFrame;
uniform bool IsAnimation;

uniform sampler2D Texture0, Texture1;

float ClampedSin(float _value);
vec4 ColourFromTexturesORWhite(vec2 _texCoords);

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
