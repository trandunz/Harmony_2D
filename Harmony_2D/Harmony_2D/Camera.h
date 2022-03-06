#pragma once
#include "Helper.h"

class Camera
{
public:
    Camera(std::map<int, bool>& _keyMap, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f));
    ~Camera();

    inline glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }
    inline glm::mat4 GetProjectionMatrix()
    {
        return glm::perspective(glm::radians(m_Zoom), 1080.0f / 1080.0f, 0.1f, 100.0f);
    }

    void Input();
    void Movement(const long double& _dt);
    void ProcessMouse(const float& _xOffset, const float& _yOffset);
    void ProcessScroll(const float& _yoffset);
    glm::vec3 GetPosition() { return m_Position; };
private:
    void UpdateCameraVectors();
    bool UpdatePosition(const long double& _dt);

    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
    float m_MoveSpeed = 3.0f;
    float m_Sensitivity = 0.5f;
    float m_Zoom = 45.0f;

    std::map<int, bool>* m_KeyPresses = nullptr;

    glm::vec3 m_InputVec;
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_WorldUp;
    glm::vec3 m_Right;
};

