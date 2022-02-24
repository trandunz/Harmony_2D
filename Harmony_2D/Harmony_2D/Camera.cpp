#include "Camera.h"

Camera::Camera(std::map<int, bool>& _keyMap, glm::vec3 _position, glm::vec3 _up, glm::vec3 _front)
{
    m_KeyPresses = &_keyMap;
    m_Position = _position;
    m_WorldUp = _up;
    m_Front = _front;

    UpdateCameraVectors();
}

Camera::~Camera()
{
    m_KeyPresses = nullptr;
}

void Camera::Movement(const long double& _dt)
{
    UpdatePosition(_dt);
}

bool Camera::UpdatePosition(const long double& _dt)
{
    bool moved = false;
    float x = m_InputVec.x * m_MoveSpeed * _dt;
    float y = m_InputVec.y * m_MoveSpeed * _dt;
    float z = m_InputVec.z * m_MoveSpeed * _dt;

    if (x >= 0.000000001f || x <= -0.000000001f)
    {
        m_Position += m_Right * x;
        moved = true;
    }
    if (y >= 0.000000001f || y <= -0.000000001f)
    {
        m_Position += m_Front * y;
        moved = true;
    }
    if (z >= 0.000000001f || z <= -0.000000001f)
    {
        m_Position += m_Front * z;
        moved = true;
    }

    return moved;
}

void Camera::Input()
{
    // Reset Input Vec
    m_InputVec.x = 0.0f;
    m_InputVec.y = 0.0f;
    m_InputVec.z = 0.0f;

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

void Camera::ProcessMouse(const float& xOffset, const float& yOffset)
{
    m_Yaw += xOffset * (m_Sensitivity / 10);
    m_Pitch += yOffset * (m_Sensitivity / 10);

    if (m_Pitch > 89.0f)
        m_Pitch = 89.0f;
    if (m_Pitch < -89.0f)
        m_Pitch = -89.0f;

    UpdateCameraVectors();
}

void Camera::ProcessScroll(const float& yoffset)
{
    m_Zoom -= yoffset;

    if (m_Zoom < 1.0f)
        m_Zoom = 1.0f;
    if (m_Zoom > 45.0f)
        m_Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 newFront = 
    {
        cos(glm::radians(m_Yaw))* cos(glm::radians(m_Pitch)),
        sin(glm::radians(m_Pitch)),
        sin(glm::radians(m_Yaw))* cos(glm::radians(m_Pitch))
    };
    m_Front = glm::normalize(newFront);
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
