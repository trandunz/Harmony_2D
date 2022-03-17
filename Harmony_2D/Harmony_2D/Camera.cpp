#include "Camera.h"

Camera::Camera(std::map<int, bool>& _keyMap, glm::vec3 _position)
{
    m_KeyPresses = &_keyMap;
    m_Position = _position;
}

Camera::~Camera()
{
    m_KeyPresses = nullptr;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::ortho((float)-1080 / 2, (float)1080 / 2, (float)-1080 / 2, (float)1080 / 2, 0.1f, 100.0f);
}

glm::mat4 Camera::GetPVMatrix()
{
    return GetProjectionMatrix() * GetViewMatrix();
}

void Camera::Movement(const long double& _dt)
{
    UpdatePosition(_dt);
}

bool Camera::UpdatePosition(const long double& _dt)
{
    bool moved = false;
    float x = m_InputVec.x * m_MoveSpeed * 1080;
    float y = m_InputVec.y * m_MoveSpeed * 1080;

    if (x >= 0.000000001f || x <= -0.000000001f)
    {
        m_Position += m_Right * x *(float)_dt;
        moved = true;
    }
    if (y >= 0.000000001f || y <= -0.000000001f)
    {
        m_Position += m_Up * y * (float)_dt;
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
