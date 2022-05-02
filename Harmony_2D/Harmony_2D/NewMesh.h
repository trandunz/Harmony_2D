#pragma once
#include "Helper.h"
#include "ShaderLoader.h"
class NewMesh
{
public:
	NewMesh(SHAPE _shape);
	NewMesh(unsigned int _numberOfSides);
	~NewMesh();

	void Draw();

private:
	void CreateShapeVertices(SHAPE _shape);
	void CreateShapeIndices(SHAPE _shape);

	void CreatePolygonVertices(unsigned int _numberOfSides);
	void CreatePolygonIndices(unsigned int _numberOfSides);

	float ToTexCoord(float& _position);

	std::vector<unsigned int> m_Indices{};
	std::vector<Vertex> m_Vertices{};

	GLuint m_VertexArrayID{ 0 };
	GLuint m_VertexBufferID{ 0 };
	GLuint m_IndexBufferID{ 0 };
};

