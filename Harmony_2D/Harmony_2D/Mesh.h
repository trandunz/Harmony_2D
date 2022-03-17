#pragma once
#include "ShaderLoader.h"
#include "Camera.h"
#include "TextureLoader.h"

class Mesh
{
public:
	Mesh(Camera& _camera, double& _deltaTime, unsigned&& _numberOfSides = 4, std::vector<Texture>&& _textures = {});
	Mesh(Camera& _camera, double& _deltaTime, unsigned&& _numberOfSides = 4, unsigned&& _numberOfAnimationFrames = 8, std::vector<Texture>&& _textures = {});
	~Mesh();

	void Draw();

	inline Transform& GetTransform() { return m_Transform; }
	void SetPosition(glm::vec3&& _newPos);
	void SetScale(glm::vec3&& _newScale);
	void Scale(glm::vec3&& _scaleFactor);
	void SetScaleToAnimationFrameSize();
	
	void ToggleAnimating();
	bool IsAnimating();
	void SetAnimationFrame(unsigned _frame);
private:
	void Init();

	void ScaleToTexture();
	void GeneratePolygonIndices(const int _numberOfSides = 6);
	void GeneratePolygonVertices(const int _numberOfSides = 6);
	void GenerateGenericQuadVertices();
	void GenerateGenericQuadIndices();

	float ToTexCoord(float _position);

	GLuint m_ShaderID;
	GLuint m_VertBufferID;
	GLuint m_IndexBufferID;
	GLuint m_VertexArrayID;
	GLuint m_UniformBufferID;
	bool m_Animated = false;
	bool m_Animating = false;
	double* m_DeltaTime = nullptr;
	unsigned m_NumberOfSides = 4;
	unsigned m_CurrentAnimationFrame = 0;
	unsigned m_NumberOfAnimationFrames = 8;
	float m_FrameTime_s = 0.09f;
	float m_AnimationTimer = 0.0f;

	glm::mat4 m_PVMatrix;
	Transform m_Transform;

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned> m_Indices;
	std::vector<Texture> m_ActiveTextures;

	Camera* m_Camera = nullptr;
};

