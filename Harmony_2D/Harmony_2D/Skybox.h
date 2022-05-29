#pragma once
#include "Mesh.h"
#include "Camera.h"
class Skybox
{
public:
	Skybox(Camera* _activeCamera, Mesh* _mesh, Texture _cubemapTexture);
	~Skybox();

	void Update(float& _dt);
	void Draw();

	void SetTexture(Texture _cubemapTexture);

	void SetMesh(Mesh* _mesh);
	void SetActiveCamera(Camera* _camera);

	/// <summary>
	/// Sets the position of the Skybox
	/// </summary>
	/// <param name="_newPosition"></param>
	void SetTranslation(glm::vec3 _newPosition);
	/// <summary>
	/// Translates the position of the Skybox by an amount
	/// </summary>
	/// <param name="_translation"></param>
	void Translate(glm::vec3 _translation);

	/// <summary>
	/// Sets the rotation of the Skybox
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void SetRotation(glm::vec3 _axis, float _degrees);
	/// <summary>
	/// Rotates the Skybox by an amount
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void Rotate(glm::vec3 _axis, float _degrees);

	/// <summary>
	/// Sets the scale of the Skybox
	/// </summary>
	/// <param name="_newScale"></param>
	void SetScale(glm::vec3 _newScale);
	/// <summary>
	/// Scales the Skybox by amount
	/// </summary>
	/// <param name="_scaleFactor"></param>
	void Scale(glm::vec3 _scaleFactor);

private:
	Transform m_Transform{};
	GLuint m_ShaderID{ 0 };
	Camera* m_ActiveCamera{ nullptr };
	Texture m_CubemapTexture{};
	Mesh* m_Mesh = nullptr;
};

