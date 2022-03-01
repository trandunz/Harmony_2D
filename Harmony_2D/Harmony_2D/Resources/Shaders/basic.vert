#version 460 core

layout (location = 0) in vec3 l_position;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 Model;

void main()
{
	gl_Position = projection * view * Model * vec4(l_position,1.0f);
}