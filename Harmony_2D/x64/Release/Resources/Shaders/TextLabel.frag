#version 460 core

layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;

// Outside Variables Passed In As 'Uniforms'
uniform sampler2D Texture;
uniform vec4 Colour;

// Main function that gets called per vertex fragment.
void main()
{
    float alpha = texture2D(Texture, TexCoords).r;
    FragColor = vec4(Colour, alpha);
} 