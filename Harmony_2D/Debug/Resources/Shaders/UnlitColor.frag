// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : UnlitColor.frag
// Description : Generic fragment shader for handling unlit coloured Meshes
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;

// Outside Variables Passed In As 'Uniforms'
uniform vec3 Color;

// Main function that gets called per vertex fragment.
void main()
{
    FragColor = vec4(Color,1.0f);
}
