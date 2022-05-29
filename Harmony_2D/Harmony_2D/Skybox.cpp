#include "Skybox.h"

Skybox::Skybox(Camera* _activeCamera, Mesh* _mesh, Texture _cubemapTexture)
{
	m_Mesh = _mesh;
	m_ActiveCamera = _activeCamera;
	m_CubemapTexture = _cubemapTexture;
	SetScale({1000,1000,1000});

	m_ShaderID = ShaderLoader::CreateShader("Skybox.vert","Skybox.frag");
}

Skybox::~Skybox()
{
	if (m_Mesh)
		m_Mesh = nullptr;
	if (m_ActiveCamera)
		m_ActiveCamera = nullptr;
}

void Skybox::Update(float& _dt)
{
	
}

void Skybox::Draw()
{
	if (m_Mesh)
	{
		glUseProgram(m_ShaderID);

		// Set Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture.ID);
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);

		// Set PVM Matrix
		if (m_ActiveCamera)
			ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", m_ActiveCamera->GetPVMatrix() * m_Transform.transform);
		
		m_Mesh->Draw();
		
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glUseProgram(0);
	}
}

void Skybox::SetTexture(Texture _cubemapTexture)
{
	m_CubemapTexture = _cubemapTexture;
}

void Skybox::SetMesh(Mesh* _mesh)
{
	m_Mesh = _mesh;
}

void Skybox::SetActiveCamera(Camera* _camera)
{
	m_ActiveCamera = _camera;
}

void Skybox::SetTranslation(glm::vec3 _newPosition)
{
	m_Transform.translation = _newPosition;
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::Translate(glm::vec3 _translation)
{
	m_Transform.translation += _translation;
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::SetRotation(glm::vec3 _axis, float _degrees)
{
	m_Transform.rotation_axis = _axis;
	m_Transform.rotation_value = glm::radians(_degrees);
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::Rotate(glm::vec3 _axis, float _degrees)
{
	m_Transform.rotation_axis = _axis;
	m_Transform.rotation_value += glm::radians(_degrees);
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::SetScale(glm::vec3 _newScale)
{
	m_Transform.scale = _newScale;
	UpdateModelValueOfTransform(m_Transform);
}

void Skybox::Scale(glm::vec3 _scaleFactor)
{
	m_Transform.scale *= _scaleFactor;
	UpdateModelValueOfTransform(m_Transform);
}
