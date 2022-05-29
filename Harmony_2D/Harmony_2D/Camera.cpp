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

Camera::Camera(glm::ivec2& _windowSize, glm::vec3 _position)
{
    m_Position = _position;
    m_WindowSize = &_windowSize;
}

Camera::~Camera()
{
    m_WindowSize = nullptr;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return m_Perspective 
        ? 
        glm::perspective(
            glm::radians(m_Fov), 
            (float)m_WindowSize->x / 
            (float)m_WindowSize->y, 
            m_NearPlane, 
            m_FarPlane) 
        : 
        glm::ortho(
            (float)-m_WindowSize->x / 2, 
            (float)m_WindowSize->x / 2, 
            (float)-m_WindowSize->y / 2, 
            (float)m_WindowSize->y / 2, 
            m_NearPlane, 
            m_FarPlane);
}

void Camera::UpdateRotationVectors()
{
    m_Front = glm::normalize(glm::vec3{
        cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw)),
        sin(glm::radians(m_Pitch)),
        cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw))
        });
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    m_Right = glm::normalize(glm::cross(m_Front, { 0,1,0 }));
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

void Camera::SetLookSensitivity(float _newSenstivity)
{
    m_LookSensitivity = _newSenstivity;
}

float Camera::GetLookSensitivity()
{
    return m_LookSensitivity;
}

void Camera::Movement(float& _dt)
{
    UpdateRotationVectors();
    UpdatePosition(_dt);
}

void Camera::MouseLook(float& _dt, glm::vec2 _mousePos)
{
    if (m_LastMousePos == glm::vec2(0, 0))
        m_LastMousePos = _mousePos;

    m_Yaw += (m_LookSensitivity * (_mousePos.x - m_LastMousePos.x) * _dt);
    m_Pitch += (m_LookSensitivity * (m_LastMousePos.y - _mousePos.y) * _dt);

    if (m_Pitch > 89.0f) 
        m_Pitch = 89.0f;
    if (m_Pitch < -89.0f) 
        m_Pitch = -89.0f;

    m_LastMousePos = _mousePos;
}

void Camera::UpdatePosition(float& _dt)
{
    float x;
    float y;
    float z;

    if (m_Perspective)
    {
        x = m_InputVec.x * m_MoveSpeed;
        y = m_InputVec.y * m_MoveSpeed;
        z = m_InputVec.z * m_MoveSpeed;
    }
    else
    {
        x = m_InputVec.x * m_MoveSpeed * m_WindowSize->x;
        y = m_InputVec.y * m_MoveSpeed * m_WindowSize->x;
        z = m_InputVec.z * m_MoveSpeed * m_WindowSize->x;
    }

    if (x != 0)
    {
        m_Position += m_Right * x * _dt;
    }
    if (y != 0)
    {
        m_Position += glm::vec3{0,1,0} * y * _dt;
    }
    if (z != 0)
    {
        m_Position += m_Front * z * _dt;
    }
}

void Camera::Movement_Capture(KEYMAP& _keymap)
{
    // Reset Input Vec
    m_InputVec = {};

    for (auto& item : (_keymap))
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
            case GLFW_KEY_Q:
            {
                m_InputVec.y = -1;
                break;
            }
            case GLFW_KEY_E:
            {
                m_InputVec.y = 1;
                break;
            }
            case GLFW_KEY_W:
            {
                m_InputVec.z = 1;
                break;
            }
            case GLFW_KEY_S:
            {
                m_InputVec.z = -1;
                break;
            }
            
            default:
                break;
            }
        }
    }

    glm::normalize(m_InputVec);
}
