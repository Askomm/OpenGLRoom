#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    glm::vec3 Position;

    Camera(glm::vec3 position = glm::vec3(0.0f, 1.5f, 3.0f));

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(
        bool forward,
        bool backward,
        bool left,
        bool right,
        float deltaTime
    );

private:
    float MovementSpeed = 3.0f;
};