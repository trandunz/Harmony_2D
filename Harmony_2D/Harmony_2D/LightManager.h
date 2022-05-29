#pragma once
#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

struct PointLight
{
    glm::vec3 Position{0,0,0};
    glm::vec3 Color{1.0f,1.0f,1.0f};
    float SpecularStrength = 1.0f;

    float AttenuationLinear = 0.045f;
    float AttenuationExponent = 0.0075f;
};

struct DirectionalLight
{
    glm::vec3 Direction;
    glm::vec3 Color;
    float SpecularStrength;
};

class LightManager
{
public:
    LightManager(Camera& _activeCamera, int _maxPointLights = 1, int _maxDirectionalLights = 1);
    ~LightManager();

    void Draw();

    void SetLightMesh(Mesh* _mesh);

    void CreatePointLight(PointLight _newLight);
    void CreateDirectionalLight(DirectionalLight _newLight);

    std::vector<PointLight>& GetPointLights();
    std::vector<DirectionalLight>& GetDirectionalLights();
private:
    Camera* m_ActiveCamera{ nullptr };
    GLuint m_UnlitMeshShaderID{ 0 };
    Mesh* m_LightMesh{nullptr};
    int m_MaxPointLights{1};
    int m_MaxDirectionalLights{ 1 };
    std::vector<PointLight> m_PointLights{};
    std::vector<DirectionalLight> m_DirectionalLights{};
};

