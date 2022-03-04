#version 460 core

layout (location = 0) in vec3 l_position;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec3 Position;
out mat4 Model_pass;
out mat4 View_pass;
out mat4 Proj_pass;

uniform mat4 Model;

void main()
{
    Position = l_position;
    Model_pass = Model;
    View_pass = view;
    Proj_pass = projection;
	gl_Position = projection * view * Model * vec4(l_position,1.0f);
}