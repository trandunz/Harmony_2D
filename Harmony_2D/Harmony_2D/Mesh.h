#pragma once
#include "ShaderLoader.h"
#include "Camera.h"
#include "TextureLoader.h"

class Mesh
{
public:
	Mesh(Camera& _camera, double& _deltaTime, unsigned&& _numberOfSides = 4, std::vector<Texture>&& _textures = {}, bool&& _animated = false);
	~Mesh();
	void Init();
	void Draw();

	void SetPosition(glm::vec3&& _newPos);
	void SetScale(glm::vec3&& _newScale);
	inline Transform& GetTransform() { return m_Transform; }
private:
	GLuint ShaderID;
	GLuint VertBufferID;
	GLuint IndexBufferID;
	GLuint VertexArrayID;
	GLuint UniformBufferID;
	bool m_Animated = false;
	double* m_DeltaTime = nullptr;
	unsigned m_NumberOfSides = 4;
	unsigned m_CurrentAnimationFrame = 1;
	unsigned m_NumberOfAnimationFrames = 8;
	float m_FrameTime_s = 0.09f;
	float m_AnimationTimer = 0.0f;

	glm::mat4 m_PVMatrix;

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned> m_Indices;
	std::vector<Texture> m_ActiveTextures;

	Camera* m_Camera = nullptr;

	Transform m_Transform;

	void ScaleToTexture();
	void GeneratePolygonIndices(const int _numberOfSides = 6);
	void GeneratePolygonVertices(const int _numberOfSides = 6);
	void GenerateGenericQuadVertices();
	void GenerateGenericQuadIndices();
	float ToTexCoord(float _position);
};

