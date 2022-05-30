#include "LightManager.h"

LightManager::LightManager(Camera& _activeCamera, int _maxPointLights, int _maxDirectionalLights, int _maxSpotLights)
{
	m_MaxPointLights = _maxPointLights;
	m_MaxDirectionalLights = _maxDirectionalLights;
	m_MaxSpotLights = _maxSpotLights;
	m_ActiveCamera = &_activeCamera;
	m_UnlitMeshShaderID = ShaderLoader::CreateShader("SingleTexture.vert","UnlitColor.frag");
}

LightManager::~LightManager()
{
	m_LightMesh = nullptr;
	m_ActiveCamera = nullptr;
	m_PointLights.clear();
	m_DirectionalLights.clear();
	m_SpotLights.clear();
}

void LightManager::Draw()
{
	if (m_LightMesh)
	{
		glUseProgram(m_UnlitMeshShaderID);
		for (auto& light : m_PointLights)
		{
			ShaderLoader::SetUniformMatrix4fv(std::move(m_UnlitMeshShaderID), "PVMMatrix", m_ActiveCamera->GetPVMatrix() * glm::translate(glm::mat4(1), light.Position));
			ShaderLoader::SetUniform3fv(std::move(m_UnlitMeshShaderID), "Color", std::move(light.Color));
			m_LightMesh->Draw();
		}
		glUseProgram(0);
	}
}

void LightManager::SetLightMesh(Mesh* _mesh)
{
	m_LightMesh = _mesh;
}

void LightManager::CreatePointLight(PointLight _newLight)
{
	if ((int)m_PointLights.size() < m_MaxPointLights)
		m_PointLights.emplace_back(_newLight);
}

void LightManager::CreateDirectionalLight(DirectionalLight _newLight)
{
	if ((int)m_DirectionalLights.size() < m_MaxDirectionalLights)
		m_DirectionalLights.emplace_back(_newLight);
}

void LightManager::CreateSpotLight(SpotLight _newLight)
{
	if ((int)m_SpotLights.size() < m_MaxSpotLights)
		m_SpotLights.emplace_back(_newLight);
}

std::vector<PointLight>& LightManager::GetPointLights()
{
	return m_PointLights;
}

std::vector<DirectionalLight>& LightManager::GetDirectionalLights()
{
	return m_DirectionalLights;
}

std::vector<SpotLight>& LightManager::GetSpotLights()
{
	return m_SpotLights;
}
