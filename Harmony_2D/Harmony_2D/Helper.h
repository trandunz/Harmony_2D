// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Helper.h 
// Description : Helper Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

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

#define PI 3.141592654
#define TWOPI 6.283185307186
#define THREEPI 9.42477796077

/// <summary>
/// ShaderProgramLocation struct that encapuslates the file paths of a programs shaders.
/// </summary>
struct ShaderProgramLocation
{
    const char* vertShader;
    const char* fragShader;
};

/// <summary>
/// UniformLocation struct that encapsulates the uniforms program and location.
/// </summary>
struct UniformLocation
{
    GLuint program;
    const char* location;
};

/// <summary>
/// Vertex struct that encapsulates Positional data and textureCoordinates
/// </summary>
struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
};

/// <summary>
/// Transform struct that encapsulates positional data such as translation, rotation and scale.
/// </summary>
struct Transform
{
    glm::mat4 tranform = glm::mat4(1);
    glm::vec3 rotation_axis = { 0,0,0 };
    glm::vec3 translation = {0,0,0};
    glm::vec3 scale = {1,1,1};
    GLfloat rotation_value = 0.0f;
};

/// <summary>
/// Texture Struct that contains the ID of the texture, 
/// Its dimentions and its filePath.
/// </summary>
struct Texture
{
	GLuint ID = 0;
	glm::vec2 Dimensions{ 0 };
	const char* FilePath = "";
};

/// <summary>
/// Updates the provided transform's model matrix with 
/// its positional, rotational and scaling values.
/// </summary>
/// <param name="_transform"></param>
/// <returns></returns>
static inline glm::mat4& UpdateModelValueOfTransform(Transform& _transform)
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

/// <summary>
/// Prints the given string withought a heap allocation to the console output stream.
/// </summary>
/// <param name="_string"></param>
static inline void Print(std::string_view _string)
{
	std::cout << _string << std::endl;
}

/// <summary>
/// Prints the given integer to the console output stream.
/// </summary>
/// <param name="_int"></param>
static inline void Print(const int&& _int)
{
	std::cout << _int << std::endl;
}

/// <summary>
/// Prints the given float to the console output Stream.
/// </summary>
/// <param name="_float"></param>
static inline void Print(const float&& _float)
{
	std::cout << _float << std::endl;
}

