#pragma once
#include "ShaderLoader.h"
#include "Camera.h"

class Mesh
{
public:
	Mesh(GLuint _textureID);
	Mesh(Camera& _camera);
	~Mesh();
	void Init(GLuint _screenTextureID);
	void Init();
	void Draw(float _depth = 1);

	inline Transform& GetTransform() { return m_Transform; }
private:
	GLuint ShaderID;
	GLuint VertBufferID;
	GLuint IndexBufferID;
	GLuint VertexArrayID;
	GLuint UniformBufferID;
	int m_ObjectID = 1;

	glm::mat4 ProjectionMat;
	glm::mat4 ViewMat;

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned> m_Indices;

	Camera* m_Camera = nullptr;

	Transform m_Transform;
};

