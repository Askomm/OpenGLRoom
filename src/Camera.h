#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    glm::vec3 Position;

    Camera(glm::vec3 position = glm::vec3(0.0f, 1.6f, 6.5f));

    glm::mat4 GetViewMatrix() const;

    void ProcessKeyboard(
        bool forward,
        bool backward,
        bool left,
        bool right,
        float deltaTime
    );

    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yOffset);

    float GetZoom() const;

private:
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    void UpdateCameraVectors();
};
