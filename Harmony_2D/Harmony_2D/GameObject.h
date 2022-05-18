#pragma once
#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"
class GameObject
{
public:
	/// <summary>
	/// GameObject contructor
	/// </summary>
	/// <param name="_camera"></param>
	/// <param name="_deltaTime"></param>
	/// <param name="_position"></param>
	GameObject(Camera& _camera, double& _deltaTime, glm::vec3 _position);
	
	/// <summary>
	/// GameObject Destructor
	/// </summary>
	~GameObject();

	/// <summary>
	/// Handles Input Actions
	/// </summary>
	/// <param name="_keypresses"></param>
	void KeyboardInput(std::map<int, bool>& _keypresses);

	/// <summary>
	/// Update function for GameObject.
	/// Should be called every frame.
	/// </summary>
	void Update();

	/// <summary>
	/// Draws The gameobject if it has a mesh attached
	/// </summary>
	void Draw();

	/// <summary>
	/// Sets the name of the gameobject for gameobject comparisons
	/// </summary>
	/// <param name="_newName"></param>
	void SetName(std::string_view _newName);
	/// <summary>
	/// Returns the name of the gameobject for gameobject comparisons
	/// </summary>
	/// <returns></returns>
	std::string_view GetName();

	/// <summary>
	/// Attaches a mesh to be used for drawing.
	/// </summary>
	/// <param name="_mesh"></param>
	void SetMesh(Mesh* _mesh);
	/// <summary>
	/// Returns the attached mesh.
	/// </summary>
	/// <returns></returns>
	Mesh* GetMesh();

	/// <summary>
	/// Sets the position of the gameObject
	/// </summary>
	/// <param name="_newPosition"></param>
	void SetTranslation(glm::vec3 _newPosition);
	/// <summary>
	/// Translates the position of the gameObject by an amount
	/// </summary>
	/// <param name="_translation"></param>
	void Translate(glm::vec3 _translation);

	/// <summary>
	/// Sets the rotation of the gameObject
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void SetRotation(glm::vec3 _axis, float _degrees);
	/// <summary>
	/// Rotates the gameobject by an amount
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void Rotate(glm::vec3 _axis, float _degrees);

	/// <summary>
	/// Sets the scale of the gameobject
	/// </summary>
	/// <param name="_newScale"></param>
	void SetScale(glm::vec3 _newScale);
	/// <summary>
	/// Scales the gameObject by amount
	/// </summary>
	/// <param name="_scaleFactor"></param>
	void Scale(glm::vec3 _scaleFactor);

	/// <summary>
	/// Rotates the gameObject around the position on the given axis by specified degrees.
	/// </summary>
	/// <param name="_position"></param>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void RotateAround(glm::vec3&& _position, glm::vec3&& _axis, float&& _degrees);

	/// <summary>
	/// Sets the active camera of the gameObject
	/// </summary>
	/// <param name="_newCamera"></param>
	void SetActiveCamera(Camera& _newCamera);
	/// <summary>
	/// Returns the current active camera of the gameObject
	/// </summary>
	/// <returns></returns>
	Camera* GetActiveCamera();

	/// <summary>
	/// Sets the active textures to specified textures.
	/// </summary>
	/// <param name="_textures"></param>
	void SetActiveTextures(std::vector<Texture> _textures);
	/// <summary>
	/// Returns the vector of active textures
	/// </summary>
	/// <returns></returns>
	std::vector<Texture> GetActiveTextures();

	/// <summary>
	/// Sets the shader program to use for rendering
	/// </summary>
	/// <param name="_newShader"></param>
	void SetShader(GLuint _newShader);
	/// <summary>
	/// Returns the current shader program used for rendering
	/// </summary>
	/// <returns></returns>
	GLuint GetShader();

	/// <summary>
	/// Clears the input vector to prevent further movement e.t.c
	/// </summary>
	void ClearInputVector();
private:
	std::vector<Texture> m_ActiveTextures{};
	glm::uint m_ShaderID{0};
	glm::vec3 m_Input{};
	float m_MovementSpeed = 10.0f;
	Mesh* m_Mesh = nullptr;
	Camera* m_ActiveCamera = nullptr;
	double* m_DeltaTime = nullptr;
	Transform m_Transform{};
	std::string m_Name{"gameObject"};
};

