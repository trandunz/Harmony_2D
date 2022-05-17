#pragma once
#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"
class GameObject
{
public:
	GameObject(Camera& _camera, double& _deltaTime, glm::vec3 _position);
	~GameObject();

	void KeyboardInput(std::map<int, bool>& _keypresses);
	void Update();
	void Draw();

	void SetName(std::string_view _newName);
	std::string_view GetName();

	void SetMesh(Mesh* _mesh);
	Mesh* GetMesh();

	void SetTranslation(glm::vec3 _newPosition);
	void Translate(glm::vec3 _translation);

	void SetRotation(glm::vec3 _axis, float _degrees);
	void Rotate(glm::vec3 _axis, float _degrees);

	void SetScale(glm::vec3 _newScale);
	void Scale(glm::vec3 _scaleFactor);

	void RotateAround(glm::vec3&& _position, glm::vec3&& _axis, float&& _degrees);

	void SetActiveCamera(Camera& _newCamera);
	Camera* GetActiveCamera();

	void SetActiveTextures(std::vector<Texture> _textures);
	std::vector<Texture> GetActiveTextures();

	void SetShader(GLuint _newShader);
	GLuint GetShader();

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

