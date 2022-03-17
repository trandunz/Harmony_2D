#version 460 core

layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texCoords;

layout (std140, binding = 0) uniform Matrices
{
    mat4 PVMatrix;
};

out vec3 Position;
out vec2 TexCoords;

uniform mat4 Model;

void main()
{
    Position = l_position;
    TexCoords = l_texCoords;
	gl_Position = PVMatrix * Model * vec4(l_position,1.0f);
}