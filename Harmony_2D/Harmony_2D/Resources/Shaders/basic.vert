#version 460 core

layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texCoords;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
	mat4 model;
};

out vec3 Position;
out vec2 TexCoords;

void main()
{
	TexCoords = l_texCoords;
	Position = l_position;
	gl_Position = projection * view * model * vec4(l_position,1.0f);
}