#version 460 core

layout (location = 0) in vec3 l_position;

out vec3 Position;

void main()
{
	Position = l_position;
	gl_Position = vec4(l_position,1.0f);
}