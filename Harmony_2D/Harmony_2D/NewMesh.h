#pragma once
#include "Helper.h"
#include "ShaderLoader.h"
class NewMesh
{
public:
	NewMesh(SHAPE _shape);
	NewMesh(unsigned int _numberOfSides);
	~NewMesh();

	void Draw(Transform& _transform, GLuint _shaderID, glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix = glm::mat4(1), std::vector<Texture> _textures = {});

private:
	void CreateShapeVertices(SHAPE _shape);
	void CreateShapeIndices(SHAPE _shape);

	void CreatePolygonVertices(unsigned int _numberOfSides);
	void CreatePolygonIndices(unsigned int _numberOfSides);

	/// <summary>
	/// Converts a given positional value (e.g xPos) to texture coordinate space. (0.0f->1.0f)
	/// </summary>
	/// <param name="_position"></param>
	/// <returns></returns>
	float ToTexCoord(float& _position);

	std::vector<unsigned int> m_Indices{};
	std::vector<Vertex> m_Vertices{};

	GLuint m_VertexArrayID{ 0 };
	GLuint m_VertexBufferID{ 0 };
	GLuint m_IndexBufferID{ 0 };
};

