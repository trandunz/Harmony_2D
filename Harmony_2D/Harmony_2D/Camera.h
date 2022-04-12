// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Camera.h 
// Description : Camera Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

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
    Camera(glm::ivec2& _windowSize, std::map<int, bool>& _keyMap, glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f));
    
    /// <summary>
    /// Cleans Up Any Pointers Or Objects (e.g KeyPresses*, WidnowSize*)
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

    /// <summary>
    /// Sets the near and far plane of the camera in that order ({newNear, newFar}).
    /// </summary>
    /// <param name="_nearAndFar"></param>
    void SetNearAndFarPlane(glm::vec2 _nearAndFar);

    /// <summary>
    /// Returns the value of the nearPlane.
    /// </summary>
    /// <returns></returns>
    inline float GetNearPlane() { return m_NearPlane; }

    /// <summary>
    /// Returns the value of the farPlane.
    /// </summary>
    /// <returns></returns>
    inline float GetFarPlane() { return m_FarPlane; }

    /// <summary>
    /// Creates And Returns The Cameras View Matrix
    /// </summary>
    /// <returns></returns>
    glm::mat4 GetViewMatrix();

    /// <summary>
    /// Creates And Returns The Cameras Projection Matrix
    /// </summary>
    /// <returns></returns>
    glm::mat4 GetProjectionMatrix();
private:

    /// <summary>
    /// Updates The Cameras Position Based On Input
    /// </summary>
    /// <param name="_dt"></param>
    /// <returns></returns>
    bool UpdatePosition(const long double& _dt);

    void UpdateRotationVectors();

    float m_MoveSpeed = 10.0f;
    std::map<int, bool>* m_KeyPresses = nullptr;
    glm::ivec2* m_WindowSize = nullptr;

    bool m_Perspective = true;

    float m_NearPlane = 0.1f, m_FarPlane = 100.0f;
    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
    float m_Fov = 45.0f;
    glm::vec3 m_InputVec{ 0,0,0 };
    glm::vec3 m_Position{ 0,0,0 };
    glm::vec3 m_Front{ 0,0,-1 };
    glm::vec3 m_Up{0,1,0};
    glm::vec3 m_WorldUp{ 0,1,0 };
    glm::vec3 m_Right{ 1,0,0 };
};

