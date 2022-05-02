#include "GameObject.h"

GameObject::GameObject(Camera& _camera, double& _deltaTime, glm::vec3 _position)
{
    m_ActiveCamera = &_camera;
    m_DeltaTime = &_deltaTime;
    SetTranslation(_position);
}

GameObject::~GameObject()
{
    if (m_Mesh)
        m_Mesh = nullptr;

    if (m_ActiveCamera)
        m_ActiveCamera = nullptr;

    m_DeltaTime = nullptr;

    m_ActiveTextures.clear();
}

void GameObject::KeyboardInput(std::map<int, bool>& _keypresses)
{
    m_Input = {};
    for (auto& item : _keypresses)
    {
        if (item.second)
        {
            if (item.first == GLFW_KEY_W)
            {
                m_Input.z -= 1.0f;
            }
            if (item.first == GLFW_KEY_A)
            {
                m_Input.x -= 1.0f;
            }
            if (item.first == GLFW_KEY_S)
            {
                m_Input.z += 1.0f;
            }
            if (item.first == GLFW_KEY_D)
            {
                m_Input.x += 1.0f;
            }
            if (item.first == GLFW_KEY_Q)
            {
                m_Input.y -= 1.0f;
            }
            if (item.first == GLFW_KEY_E)
            {
                m_Input.y += 1.0f;
            }
            if (item.first == GLFW_KEY_Z)
            {
                Rotate({ 0,1,0 }, -100 * (float) * m_DeltaTime);
            }
            if (item.first == GLFW_KEY_C)
            {
                Rotate({ 0,1,0 }, 100 * (float)*m_DeltaTime);
            }
        }
    }
    glm::normalize(m_Input);
}

void GameObject::Update()
{
    if (Magnitude(m_Input) > 0)
        Translate(m_Input * (float)*m_DeltaTime * m_MovementSpeed);
}

void GameObject::Draw()
{
    if (m_Mesh)
    {
        glUseProgram(m_ShaderID);

        // Textures
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", (GLint)m_ActiveTextures.size());
        for (unsigned i = 0; i < m_ActiveTextures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[i].ID);
            ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture" + std::to_string(i), std::move(i));
        }

        // Model Matrix
        ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "Model", std::move(m_Transform.transform));

        // Projection * View Matrix
        ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMatrix", m_ActiveCamera->GetPVMatrix());

        // Elapsed Time
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "Time", (float)glfwGetTime());

        // Lighting

        // Ambient Lighting Strength
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "AmbientStength", 0.15f);

        // Ambient Lighting Strength
        ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "AmbientColor", {1,1,1});

        // Ambient Lighting Strength
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "Shininess", 32.0f);

        ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPosition", std::move(m_ActiveCamera->GetPosition()));

        m_Mesh->Draw();
    }
}

void GameObject::SetName(std::string_view _newName)
{
    m_Name = _newName;
}

std::string_view GameObject::GetName()
{
    return m_Name;
}

void GameObject::SetMesh(NewMesh* _mesh)
{
    m_Mesh = _mesh;
}

NewMesh* GameObject::GetMesh()
{
    if (m_Mesh)
        return m_Mesh;
    else
        return nullptr;
}

void GameObject::SetTranslation(glm::vec3 _newPosition)
{
    m_Transform.translation = _newPosition;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::Translate(glm::vec3 _translation)
{
    m_Transform.translation += _translation;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::SetRotation(glm::vec3 _axis, float _degrees)
{
    m_Transform.rotation_axis = _axis;
    m_Transform.rotation_value = glm::radians(_degrees);
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::Rotate(glm::vec3 _axis, float _degrees)
{
    m_Transform.rotation_axis = _axis;
    m_Transform.rotation_value += glm::radians(_degrees);
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::SetScale(glm::vec3 _newScale)
{
    m_Transform.scale = _newScale;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::Scale(glm::vec3 _scaleFactor)
{
    m_Transform.scale *= _scaleFactor;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::RotateAround(glm::vec3&& _position, glm::vec3&& _axis, float&& _degrees)
{
    glm::vec3 direction = glm::abs(_position - m_Transform.translation);
    m_Transform.transform = glm::translate(m_Transform.transform, direction);
    m_Transform.transform = glm::rotate(m_Transform.transform, _degrees, _axis);
    m_Transform.transform = glm::translate(m_Transform.transform, -direction);
}

void GameObject::SetActiveCamera(Camera& _newCamera)
{
    m_ActiveCamera = &_newCamera;
}

Camera* GameObject::GetActiveCamera()
{
    if (m_ActiveCamera)
        return m_ActiveCamera;
    else
        return nullptr;
}

void GameObject::SetActiveTextures(std::vector<Texture> _textures)
{
    m_ActiveTextures = _textures;
}

std::vector<Texture> GameObject::GetActiveTextures()
{
    return m_ActiveTextures;
}

void GameObject::SetShader(GLuint _newShader)
{
    m_ShaderID = _newShader;
}

GLuint GameObject::GetShader()
{
    return m_ShaderID;
}
