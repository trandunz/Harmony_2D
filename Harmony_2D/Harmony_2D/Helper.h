#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

struct ShaderProgramLocation
{
    const char* vertShader;
    const char* geoShader;
    const char* fragShader;
};

struct UniformLocation
{
    GLuint program;
    const char* location;
};

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
};

struct Transform
{
    glm::mat4 tranform = glm::mat4(1);
    glm::vec3 rotation_axis = { 0,0,0 };
    glm::vec3 translation = {0,0,0};
    glm::vec3 scale = {1,1,1};
    GLfloat rotation_value = 0.0f;
};

static inline glm::mat4& UpdateModelFromTransform(Transform& _transform)
{
	_transform.tranform = glm::mat4(1);
	_transform.tranform = glm::translate(_transform.tranform, _transform.translation);
	if (_transform.rotation_axis.x > 0 ||
		_transform.rotation_axis.y > 0 ||
		_transform.rotation_axis.z > 0)
	{
		_transform.tranform = glm::rotate(_transform.tranform, _transform.rotation_value, _transform.rotation_axis);
	}
	_transform.tranform = glm::scale(_transform.tranform, _transform.scale);
	return _transform.tranform;
}

static inline void Print(std::string_view _string)
{
	std::cout << _string << std::endl;
}

