#pragma once
#include "Helper.h"

class Camera
{
public:
    /// <summary>
    /// Camera Constructor Initalizes Paramter Values
    /// </summary>
    /// <param name="_keyMap"></param>
    /// <param name="_position"></param>
    Camera(std::map<int, bool>& _keyMap, glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f));
    
    /// <summary>
    /// Cleans Up Any Pointers Or Objects (e.g KeyPresses*)
    /// </summary>
    ~Camera();

    /// <summary>
    /// Captures Camera Input
    /// </summary>
    void Input();

    /// <summary>
    /// Handles Camera Movement Based On Input. Does Not Run If No Input.
    /// </summary>
    /// <param name="_dt"></param>
    void Movement(const long double& _dt);

    /// <summary>
    /// Returns The Camera's Position
    /// </summary>
    /// <returns></returns>
    glm::vec3 GetPosition() { return m_Position; };

    /// <summary>
    /// Creates And Returns The Projection * View Matrix
    /// </summary>
    /// <returns></returns>
    glm::mat4 GetPVMatrix();
private:

    /// <summary>
    /// Updates The Cameras Position Based On Input
    /// </summary>
    /// <param name="_dt"></param>
    /// <returns></returns>
    bool UpdatePosition(const long double& _dt);

    /// <summary>
    /// Creeates And Returns The Cameras View Matrix
    /// </summary>
    /// <returns></returns>
    glm::mat4 GetViewMatrix();

    /// <summary>
    /// Creates And Returns The Cameras Projection Matrix
    /// </summary>
    /// <returns></returns>
    glm::mat4 GetProjectionMatrix();

    float m_MoveSpeed = 1.0f;
    std::map<int, bool>* m_KeyPresses = nullptr;

    glm::vec3 m_InputVec{ 0,0,0 };
    glm::vec3 m_Position{ 0,0,0 };
    glm::vec3 m_Front{ 0,0,-1 };
    glm::vec3 m_Up{0,1,0};
    glm::vec3 m_Right{ 1,0,0 };
};

