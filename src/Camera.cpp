#include "Camera.h"

Camera::Camera(glm::vec3 position)
{
    Position = position;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(
        Position,
        Position + glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

void Camera::ProcessKeyboard(
    bool forward,
    bool backward,
    bool left,
    bool right,
    float deltaTime
)
{
    float velocity = MovementSpeed * deltaTime;

    glm::vec3 front(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraRight(1.0f, 0.0f, 0.0f);

    if (forward)
        Position += front * velocity;

    if (backward)
        Position -= front * velocity;

    if (left)
        Position -= cameraRight * velocity;

    if (right)
        Position += cameraRight * velocity;
}