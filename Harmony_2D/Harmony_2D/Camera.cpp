// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Camera.cpp 
// Description : Camera Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Camera.h"

Camera::Camera(glm::ivec2& _windowSize, std::map<int, bool>& _keyMap, glm::vec3 _position)
{
    m_KeyPresses = &_keyMap;
    m_Position = _position;
    m_WindowSize = &_windowSize;
}

Camera::~Camera()
{
    m_KeyPresses = nullptr;
    m_WindowSize = nullptr;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return m_Perspective ? glm::perspective(glm::radians(m_Fov), (float)m_WindowSize->x / (float)m_WindowSize->y, m_NearPlane, m_FarPlane) : glm::ortho((float)-m_WindowSize->x / 2, (float)m_WindowSize->x / 2, (float)-m_WindowSize->y / 2, (float)m_WindowSize->y / 2, m_NearPlane, m_FarPlane);
}

void Camera::UpdateRotationVectors()
{
    glm::vec3 newFront =
    {
        cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
        sin(glm::radians(m_Pitch)),
        sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))
    };
    m_Front = glm::normalize(newFront);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

glm::mat4 Camera::GetPVMatrix()
{
    return GetProjectionMatrix() * GetViewMatrix();
}

void Camera::SetNearAndFarPlane(glm::vec2 _nearAndFar)
{
    m_NearPlane = _nearAndFar.x;
    m_FarPlane = _nearAndFar.y;
}

void Camera::Movement(const long double& _dt)
{
    UpdateRotationVectors();
    UpdatePosition(_dt);
}

bool Camera::UpdatePosition(const long double& _dt)
{
    bool moved = false;
    float x;
    float y;

    if (m_Perspective)
    {
        x = m_InputVec.x * m_MoveSpeed;
        y = m_InputVec.y * m_MoveSpeed;

    }
    else
    {
        x = m_InputVec.x * m_MoveSpeed * m_WindowSize->x;
        y = m_InputVec.y * m_MoveSpeed * m_WindowSize->x;

    }
   
   

    if (x >= 0.000000001f || x <= -0.000000001f)
    {
        m_Position += m_Right * x *(float)_dt;
        moved = true;
    }
    if (y >= 0.000000001f || y <= -0.000000001f)
    {
        m_Position += m_Front * y * (float)_dt;
        moved = true;
    }

    return moved;
}

void Camera::Input()
{
    // Reset Input Vec
    m_InputVec.x = 0.0f;
    m_InputVec.y = 0.0f;

    for (auto& item : (*m_KeyPresses))
    {
        if (item.second == true)
        {
            switch (item.first)
            {
            case GLFW_KEY_D:
            {
                m_InputVec.x = 1;
                break;
            }
            case GLFW_KEY_A:
            {
                m_InputVec.x = -1;
                break;
            }
            case GLFW_KEY_W:
            {
                m_InputVec.y = 1;
                break;
            }
            case GLFW_KEY_S:
            {
                m_InputVec.y = -1;
                break;
            }
            default:
                break;
            }
        }
    }

    glm::normalize(m_InputVec);
}
