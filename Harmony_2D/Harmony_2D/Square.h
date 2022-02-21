#pragma once
#include "ShaderLoader.h"

struct Vertex
{
	glm::vec3 position;
};

class Square
{
public:
	Square();
	~Square();
	void Init();
	void Draw();
private:
	GLuint ShaderID;
	GLuint VertBufferID;
	GLuint IndexBufferID;
	GLuint VertexArrayID;

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned> m_Indices;
};

