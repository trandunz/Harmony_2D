#pragma once
#include "ShaderLoader.h"
#include "Camera.h"
#include "TextureLoader.h"

class Mesh
{
public:
	Mesh(GLuint _textureID);
	Mesh(Camera& _camera, double& _deltaTime);
	~Mesh();
	void Init(GLuint _screenTextureID);
	void Init();
	void Draw();

	inline Transform& GetTransform() { return m_Transform; }
private:
	GLuint ShaderID;
	GLuint VertBufferID;
	GLuint IndexBufferID;
	GLuint VertexArrayID;
	GLuint UniformBufferID;
	int m_ObjectID = 1;
	bool m_Animated = true;
	double* m_DeltaTime = nullptr;

	glm::mat4 ProjectionMat;
	glm::mat4 ViewMat;

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned> m_Indices;
	std::vector<Texture> m_ActiveTextures;

	Camera* m_Camera = nullptr;

	Transform m_Transform;

	void GenerateQuadIndices(int _numberOfQuads = 1);
};

