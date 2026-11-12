#include "Camera.h"
#include "SceneLayout.h"

#include <algorithm>

Camera::Camera(glm::vec3 position)
    : Position(position),
      Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      Up(glm::vec3(0.0f, 1.0f, 0.0f)),
      Right(glm::vec3(1.0f, 0.0f, 0.0f)),
      WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      Yaw(-104.0f),
      Pitch(0.0f),
      MovementSpeed(3.0f),
      MouseSensitivity(0.10f),
      Zoom(58.0f)
{
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(
    bool forward,
    bool backward,
    bool left,
    bool right,
    float deltaTime
)
{
    const float velocity = MovementSpeed * std::min(deltaTime,0.1f);
    const glm::vec3 heading = glm::normalize(glm::vec3(Front.x,0,Front.z));
    const glm::vec3 before = Position;

    if (forward)
        Position += heading * velocity;

    if (backward)
        Position -= heading * velocity;

    if (left)
        Position -= Right * velocity;

    if (right)
        Position += Right * velocity;

    // Keep the camera inside the room and at a sensible eye height.
    Position.x = std::clamp(Position.x, -3.4f, 3.4f);
    Position.y = 1.68f;
    Position.z = std::clamp(Position.z, -3.4f, 3.6f);
    auto inside = [&](float left, float right, float back, float front) {
        return Position.x > left && Position.x < right && Position.z > back && Position.z < front;
    };
    if (inside(-2.20f + DeskOffsetX,2.20f + DeskOffsetX,-2.48f,-.42f) || inside(-1.10f + DeskOffsetX,.15f + DeskOffsetX,-.25f,.98f)
        || inside(-3.9f,-1.80f,-3.9f,-2.75f)
        || inside(-3.95f,-2.02f,-1.04f,2.10f)) Position = before;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    if (constrainPitch)
    {
        Pitch = std::clamp(Pitch, -89.0f, 89.0f);
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
    Zoom -= yOffset;
    Zoom = std::clamp(Zoom, 25.0f, 75.0f);
}

float Camera::GetZoom() const
{
    return Zoom;
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
    front.y = glm::sin(glm::radians(Pitch));
    front.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));

    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
