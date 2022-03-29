#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec4 l_vertex;

// Output data for next Shader
out vec2 TexCoords;

// Outside Variables Passed In As 'Uniforms'
uniform mat4 ProjectionMatrix;

// Main function that gets called per vertex.
void main()
{
	TexCoords = l_vertex.zw;
	gl_Position = ProjectionMatrix * vec4(l_vertex.xy, 0.0f, 1.0f);
}