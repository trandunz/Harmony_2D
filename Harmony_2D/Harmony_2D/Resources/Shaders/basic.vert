// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : basic.vert
// Description : Generic vertex shader for handling position and textture coordinate layouts
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texCoords;

// Uniform Block at binding location 0 (shared across shader programs)
layout (std140, binding = 0) uniform Matrices
{
    mat4 PVMatrix;
};

// Output data for next Shader
out vec3 Position;
out vec2 TexCoords;

// Outside Variables Passed In As 'Uniforms'
uniform mat4 Model;

// Main function that gets called per vertex.
void main()
{
    Position = l_position;
    TexCoords = l_texCoords;
	gl_Position = PVMatrix * Model * vec4(l_position,1.0f);
}