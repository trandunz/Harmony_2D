// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Mesh.h 
// Description : Mesh Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "ShaderLoader.h"
#include "Camera.h"
#include "TextureLoader.h"

class Mesh
{
public:
	/// <summary>
	/// Non-Animated Mesh Constructor
	/// </summary>
	/// <param name="_camera"></param>
	/// <param name="_deltaTime"></param>
	/// <param name="_numberOfSides"></param>
	/// <param name="_textures"></param>
	Mesh(Camera& _camera, double& _deltaTime, unsigned&& _numberOfSides = 4, std::vector<Texture>&& _textures = {});

	Mesh(Camera& _camera, double& _deltaTime, SHAPE&& _shape, std::vector<Texture>&& _textures = {});

	Mesh(MeshData& _meshData, Camera& _camera, double& _deltaTime, std::vector<Texture>&& _textures = {});

	/// <summary>
	/// Animated Mesh Contructor
	/// </summary>
	/// <param name="_camera"></param>
	/// <param name="_deltaTime"></param>
	/// <param name="_numberOfSides"></param>
	/// <param name="_numberOfAnimationFrames"></param>
	/// <param name="_textures"></param>
	Mesh(Camera& _camera, double& _deltaTime, unsigned&& _numberOfSides = 4, unsigned&& _numberOfAnimationFrames = 8, std::vector<Texture>&& _textures = {});
	
	/// <summary>
	/// Non-Animated Mesh Constructor With Copied VAO (VBO and EBO)
	/// </summary>
	/// <param name="_camera"></param>
	/// <param name="_deltaTime"></param>
	/// <param name="_numberOfSides"></param>
	/// <param name="_textures"></param>
	Mesh(GLuint&& _vertexArrayID, Camera& _camera, double& _deltaTime, unsigned&& _numberOfSides = 4, std::vector<Texture>&& _textures = {});

	/// <summary>
	/// Animated Mesh Contructor With Copied VAO (VBO and EBO)
	/// </summary>
	/// <param name="_camera"></param>
	/// <param name="_deltaTime"></param>
	/// <param name="_numberOfSides"></param>
	/// <param name="_numberOfAnimationFrames"></param>
	/// <param name="_textures"></param>
	Mesh(GLuint&& _vertexArrayID, Camera & _camera, double& _deltaTime, unsigned&& _numberOfSides = 4, unsigned&& _numberOfAnimationFrames = 8, std::vector<Texture>&& _textures = {});

	/// <summary>
	/// Cleans Up Mesh Data
	/// </summary>
	~Mesh();

	/// <summary>
	/// Handles Drawing The Mesh. If IsAnimating() is true then animtation will play
	/// </summary>
	void Draw();

	/// <summary>
	/// Returns the Transform of the Mesh
	/// </summary>
	/// <returns></returns>
	inline Transform& GetTransform() { return m_Transform; }

	/// <summary>
	/// Sets the meshes transform translation to the specified Vec3
	/// </summary>
	/// <param name="_newPos"></param>
	void SetPosition(glm::vec3&& _newPos);

	/// <summary>
	/// Sets the meshes transform scale to the specified Vec3
	/// </summary>
	/// <param name="_newScale"></param>
	void SetScale(glm::vec3&& _newScale);

	/// <summary>
	/// Scales the meshes transform by specified amount
	/// </summary>
	/// <param name="_scaleFactor"></param>
	void Scale(glm::vec3&& _scaleFactor);

	/// <summary>
	/// Sets the meshes transform rotation to the specified values
	/// </summary>
	/// <param name="_newScale"></param>
	void SetRotation(glm::vec3&& _axis, float&& _degrees);

	/// <summary>
	/// Rotates the meshes transform by the specified amount
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_value"></param>
	void Rotate(glm::vec3&& _axis, float&& _degrees);

	void RotateAround(glm::vec3&& _position, glm::vec3&& _axis, float&& _degrees);

	/// <summary>
	/// Sets the meshes transform translation to the specified position
	/// </summary>
	/// <param name="_newPosition"></param>
	void SetTranslation(glm::vec3&& _newPosition);

	/// <summary>
	/// Translates the meshes transform by the specified vector
	/// </summary>
	/// <param name="_direction"></param>
	void Translate(glm::vec3&& _direction);

	/// <summary>
	/// Scales the mesh to the size of one animation frame. 
	/// Requires the mesh to be animated.
	/// </summary>
	void SetScaleToAnimationFrameSize();
	
	/// <summary>
	/// Toggles animating if the mesh is marked as animated.
	/// </summary>
	void ToggleAnimating();

	/// <summary>
	/// Returns true or false based on the animation state of the mesh. 
	/// Requires the mesh to be animated.
	/// </summary>
	/// <returns></returns>
	bool IsAnimating();

	/// <summary>
	/// Sets the current animation frame. 
	/// Negative Values Are counted as positive. 
	/// SpriteSheet starts at frame 0.
	/// Values larger than the maxFrameCount will be clamped to the maxFrameCount.
	/// Requires the mesh to be animated.
	/// </summary>
	/// <param name="_frame"></param>
	void SetAnimationFrame(unsigned&& _frame);

	/// <summary>
	/// Sets the speed / pause on each animation frame in seconds.
	/// This will determine the overall animation speed.
	/// </summary>
	/// <param name="_frameSpeed_s"></param>
	void SetAnimationFrameTime(float&& _frameSpeed_s);

	/// <summary>
	/// Sets the value m_TextureFadeSpeed to _newSpeed.
	/// Modulates the speed in which two textures fade between eachother.
	/// </summary>
	/// <param name="_newSpeed"></param>
	void SetTextureFadeSpeed(float&& _newSpeed);

	MeshData& GetMeshData();
private:
	/// <summary>
	/// Initializes the mesh ready for drawing. Will be called on mesh construction.
	/// </summary>
	void Init();

	/// <summary>
	/// Initializes the mesh ready for drawing. Will be called on mesh construction.
	/// </summary>
	void Init(SHAPE&& _shape);

	/// <summary>
	/// Initializes the mesh ready for drawing using the already created VAO (VBO and EBO). Will be called on mesh construction.
	/// </summary>
	void Init(GLuint& _vertexArrayID);

	/// <summary>
	/// Scales the mesh to the size of its first active texture. (In Pixels)
	/// </summary>
	void ScaleToTexture();

	/// <summary>
	/// Populates the indices array with values for a polygon of N sides.
	/// Default Side count is 6. (Hexagon)
	/// </summary>
	/// <param name="_numberOfSides"></param>
	void GeneratePolygonIndices(int&& _numberOfSides = 6);

	/// <summary>
	/// Populates the vertex array with values for a polygon of N Sides.
	/// Default Side count is 6. (Hexagon)
	/// </summary>
	/// <param name="_numberOfSides"></param>
	void GeneratePolygonVertices(int&& _numberOfSides = 6);

	/// <summary>
	/// Populates the indices array with values for a shape (e.g Cube).
	/// </summary>
	/// <param name="_shape"></param>
	void GenerateShapeIndices(SHAPE _shape = SHAPE::CUBE);

	/// <summary>
	/// Populates the vertex array with values for a shape (e.g Cube).
	/// </summary>
	/// <param name="_shape"></param>
	void GenerateShapeVertices(SHAPE _shape = SHAPE::CUBE);

	/// <summary>
	/// Populates the vertex array with values for a generic quad.
	/// </summary>
	void GenerateGenericQuadVertices();

	/// <summary>
	/// Populates the indices array with values for a generic quad.
	/// </summary>
	void GenerateGenericQuadIndices();

	/// <summary>
	/// Converts a given positional value (e.g xPos) to texture coordinate space. (0.0f->1.0f)
	/// </summary>
	/// <param name="_position"></param>
	/// <returns></returns>
	float ToTexCoord(float& _position);

	GLuint m_ShaderID;
	GLuint m_VertexBufferID;
	GLuint m_UniformBufferID;
	bool m_Animated = false;
	bool m_Animating = false;
	bool m_Copied = false;
	double* m_DeltaTime = nullptr;
	unsigned m_NumberOfSides = 4;
	MeshData m_MeshData{};
	unsigned m_CurrentAnimationFrame = 0;
	unsigned m_NumberOfAnimationFrames = 8;
	float m_FrameTime_s = 0.09f;
	float m_AnimationTimer = 0.0f;
	float m_TextureFadeSpeed = 1.0f;

	glm::mat4 m_PVMatrix;
	Transform m_Transform;

	std::vector<Texture> m_ActiveTextures;

	Camera* m_Camera = nullptr;
};

