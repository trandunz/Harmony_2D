// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : GameObject.cpp 
// Description : GameObject Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "GameObject.h"

GameObject::GameObject(Camera& _camera, double& _deltaTime, glm::vec3 _position)
{
    m_ActiveCamera = &_camera;
    m_DeltaTime = &_deltaTime;
    // Set starting position
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

void GameObject::Movement_WASDEQ(std::map<int, bool>& _keypresses)
{
    // Grab keyboard input for moving Object With WASDQE
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
                m_Input.w -= 1.0f;
            }
            if (item.first == GLFW_KEY_C)
            {
                m_Input.w += 1.0f;
            }
        }
    }
    // Normalize the input vecor.
    glm::normalize(m_Input);
}

void GameObject::Update()
{
    // If player provides input, Translate the gameobject accordingly.
    if (Magnitude((glm::vec3)m_Input) > 0)
        Translate(m_Input * (float)*m_DeltaTime * m_MovementSpeed);
    // If player provides Rotational input, rotate accordingly
    if (m_Input.w != 0)
        Rotate({ 0,1,0 }, m_Input.w * (float)*m_DeltaTime * 100);
}

void GameObject::Draw()
{
    if (m_Mesh)
    {
        // Bind shader
        glUseProgram(m_ShaderID);

        // If shader program is single texture
        if (m_ShaderLocation.vertShader == "SingleTexture.vert" && 
            m_ShaderLocation.fragShader == "SingleTexture.frag")
        {
            // Apply Texture
            if (m_ActiveTextures.size() > 0)
            {
                ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[0].ID);
                ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
            }

            // Projection * View * Model Matrix
            ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", m_ActiveCamera->GetPVMatrix() * m_Transform.transform);
        }
        
        // Draw the mesh
        m_Mesh->Draw();

        // Unbind
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
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

void GameObject::SetMesh(Mesh* _mesh)
{
    m_Mesh = _mesh;
}

Mesh* GameObject::GetMesh()
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
    // get direction to the position
    glm::vec3 direction = glm::abs(_position - m_Transform.translation);
    // Translate to wards it
    m_Transform.transform = glm::translate(m_Transform.transform, direction);
    // Rotate
    m_Transform.transform = glm::rotate(m_Transform.transform, _degrees, _axis);
    // Translate back
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

void GameObject::SetShader(const char* _vertexSource,const char* _fragmentSource)
{
    m_ShaderID = ShaderLoader::CreateShader(_vertexSource, _fragmentSource);
    m_ShaderLocation = { _vertexSource , _fragmentSource };
}

GLuint GameObject::GetShader()
{
    return m_ShaderID;
}

void GameObject::ClearInputVector()
{
    m_Input = {};
}


