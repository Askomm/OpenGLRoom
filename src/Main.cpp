#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "Camera.h"
#include "Shader.h"

namespace
{
    constexpr unsigned int SCR_WIDTH = 1000;
    constexpr unsigned int SCR_HEIGHT = 700;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    Camera camera(glm::vec3(0.0f, 1.6f, 6.5f));

    bool firstMouse = true;
    float lastMouseX = SCR_WIDTH * 0.5f;
    float lastMouseY = SCR_HEIGHT * 0.5f;

    bool lamp1On = true;
    bool lamp2On = true;

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
    };

    class Mesh
    {
    public:
        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int EBO = 0;
        unsigned int indexCount = 0;

        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
        {
            indexCount = static_cast<unsigned int>(indices.size());

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(
                GL_ARRAY_BUFFER,
                static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
                vertices.data(),
                GL_STATIC_DRAW
            );

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
                indices.data(),
                GL_STATIC_DRAW
            );

            glVertexAttribPointer(
                0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<void*>(offsetof(Vertex, Position))
            );
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(
                1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<void*>(offsetof(Vertex, Normal))
            );
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        }

        void Draw() const
        {
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }

        ~Mesh()
        {
            if (EBO) glDeleteBuffers(1, &EBO);
            if (VBO) glDeleteBuffers(1, &VBO);
            if (VAO) glDeleteVertexArrays(1, &VAO);
        }
    };

    Mesh* CreateCube()
    {
        const glm::vec3 positions[] =
        {
            // Front
            {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
            // Back
            { 0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
            // Left
            {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},
            // Right
            { 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f},
            // Top
            {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
            // Bottom
            {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f}
        };

        const glm::vec3 normals[] =
        {
            {0.0f, 0.0f, 1.0f},  {0.0f, 0.0f, -1.0f},
            {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},  {0.0f, -1.0f, 0.0f}
        };

        std::vector<Vertex> vertices;
        vertices.reserve(24);

        for (int face = 0; face < 6; ++face)
        {
            for (int i = 0; i < 4; ++i)
                vertices.push_back({positions[face * 4 + i], normals[face]});
        }

        const std::vector<unsigned int> indices =
        {
             0,  1,  2,  2,  3,  0,
             4,  5,  6,  6,  7,  4,
             8,  9, 10, 10, 11,  8,
            12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20
        };

        return new Mesh(vertices, indices);
    }

    Mesh* CreateCylinder(float radius, float height, int segments)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        const float halfHeight = height * 0.5f;
        const float pi = 3.14159265359f;

        // Side vertices
        for (int i = 0; i <= segments; ++i)
        {
            const float angle = 2.0f * pi * static_cast<float>(i) / static_cast<float>(segments);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.push_back({glm::vec3(radius * x, -halfHeight, radius * z), glm::vec3(x, 0.0f, z)});
            vertices.push_back({glm::vec3(radius * x,  halfHeight, radius * z), glm::vec3(x, 0.0f, z)});
        }

        for (int i = 0; i < segments; ++i)
        {
            const unsigned int base = static_cast<unsigned int>(i * 2);
            indices.insert(indices.end(), {
                base, base + 1, base + 3,
                base, base + 3, base + 2
            });
        }

        const unsigned int bottomStart = static_cast<unsigned int>(vertices.size());
        vertices.push_back({glm::vec3(0.0f, -halfHeight, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)});

        const unsigned int topStart = static_cast<unsigned int>(vertices.size());
        vertices.push_back({glm::vec3(0.0f, halfHeight, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)});

        for (int i = 0; i < segments; ++i)
        {
            const float a0 = 2.0f * pi * static_cast<float>(i) / static_cast<float>(segments);
            const float a1 = 2.0f * pi * static_cast<float>(i + 1) / static_cast<float>(segments);

            const unsigned int bottomA = static_cast<unsigned int>(vertices.size());
            vertices.push_back({glm::vec3(radius * std::cos(a0), -halfHeight, radius * std::sin(a0)),
                                glm::vec3(0.0f, -1.0f, 0.0f)});
            const unsigned int bottomB = static_cast<unsigned int>(vertices.size());
            vertices.push_back({glm::vec3(radius * std::cos(a1), -halfHeight, radius * std::sin(a1)),
                                glm::vec3(0.0f, -1.0f, 0.0f)});
            indices.insert(indices.end(), {bottomStart, bottomB, bottomA});

            const unsigned int topA = static_cast<unsigned int>(vertices.size());
            vertices.push_back({glm::vec3(radius * std::cos(a0), halfHeight, radius * std::sin(a0)),
                                glm::vec3(0.0f, 1.0f, 0.0f)});
            const unsigned int topB = static_cast<unsigned int>(vertices.size());
            vertices.push_back({glm::vec3(radius * std::cos(a1), halfHeight, radius * std::sin(a1)),
                                glm::vec3(0.0f, 1.0f, 0.0f)});
            indices.insert(indices.end(), {topStart, topA, topB});
        }

        return new Mesh(vertices, indices);
    }

    Mesh* CreateFrustum(float bottomRadius, float topRadius, float height, int segments)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        const float halfHeight = height * 0.5f;
        const float pi = 3.14159265359f;
        const float slope = (bottomRadius - topRadius) / height;

        for (int i = 0; i <= segments; ++i)
        {
            const float angle = 2.0f * pi * static_cast<float>(i) / static_cast<float>(segments);
            const float c = std::cos(angle);
            const float s = std::sin(angle);

            glm::vec3 normal = glm::normalize(glm::vec3(c, slope, s));

            vertices.push_back({glm::vec3(bottomRadius * c, -halfHeight, bottomRadius * s), normal});
            vertices.push_back({glm::vec3(topRadius * c, halfHeight, topRadius * s), normal});
        }

        for (int i = 0; i < segments; ++i)
        {
            const unsigned int base = static_cast<unsigned int>(i * 2);
            indices.insert(indices.end(), {
                base, base + 1, base + 3,
                base, base + 3, base + 2
            });
        }

        return new Mesh(vertices, indices);
    }

    Mesh* CreateSphere(float radius, int segments, int rings)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        const float pi = 3.14159265359f;

        for (int y = 0; y <= rings; ++y)
        {
            const float v = static_cast<float>(y) / static_cast<float>(rings);
            const float phi = pi * v;

            for (int x = 0; x <= segments; ++x)
            {
                const float u = static_cast<float>(x) / static_cast<float>(segments);
                const float theta = 2.0f * pi * u;

                glm::vec3 normal(
                    std::sin(phi) * std::cos(theta),
                    std::cos(phi),
                    std::sin(phi) * std::sin(theta)
                );

                vertices.push_back({radius * normal, normal});
            }
        }

        const int rowSize = segments + 1;
        for (int y = 0; y < rings; ++y)
        {
            for (int x = 0; x < segments; ++x)
            {
                const unsigned int a = static_cast<unsigned int>(y * rowSize + x);
                const unsigned int b = a + 1;
                const unsigned int c = a + static_cast<unsigned int>(rowSize);
                const unsigned int d = c + 1;

                indices.insert(indices.end(), {a, c, b, b, c, d});
            }
        }

        return new Mesh(vertices, indices);
    }

    void DrawObject(
        const Mesh& mesh,
        Shader& shader,
        const glm::mat4& model,
        const glm::vec3& color,
        bool checker = false,
        const glm::vec3& emissionColor = glm::vec3(0.0f),
        float emissionStrength = 0.0f,
        float shininess = 32.0f
    )
    {
        shader.SetMat4("model", model);
        shader.SetVec3("objectColor", color);
        shader.SetVec3("emissionColor", emissionColor);
        shader.SetFloat("emissionStrength", emissionStrength);
        shader.SetBool("useChecker", checker);
        shader.SetFloat("checkerScale", 1.5f);
        shader.SetFloat("shininess", shininess);
        mesh.Draw();
    }

    glm::mat4 Transform(
        const glm::vec3& position,
        const glm::vec3& scale,
        float rotationY = 0.0f
    )
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, scale);
        return model;
    }

    void DrawLamp(
        Mesh& cylinder,
        Mesh& frustum,
        Mesh& sphere,
        Shader& shader,
        float x,
        float z,
        bool isOn
    )
    {
        const glm::vec3 metal(0.16f, 0.17f, 0.18f);
        const glm::vec3 shade(0.72f, 0.48f, 0.24f);
        const glm::vec3 bulb = isOn
            ? glm::vec3(1.0f, 0.73f, 0.28f)
            : glm::vec3(0.17f, 0.11f, 0.05f);

        DrawObject(
            frustum, shader,
            Transform(glm::vec3(x, 1.25f, z), glm::vec3(0.42f, 0.12f, 0.42f)),
            metal, false, glm::vec3(0.0f), 0.0f, 64.0f
        );

        DrawObject(
            cylinder, shader,
            Transform(glm::vec3(x, 1.52f, z), glm::vec3(1.0f, 0.55f, 1.0f)),
            metal, false, glm::vec3(0.0f), 0.0f, 64.0f
        );

        DrawObject(
            frustum, shader,
            Transform(glm::vec3(x, 1.95f, z), glm::vec3(1.05f, 0.75f, 1.05f)),
            shade, false, glm::vec3(0.05f, 0.02f, 0.01f), 0.0f, 24.0f
        );

        DrawObject(
            sphere, shader,
            Transform(glm::vec3(x, 2.10f, z), glm::vec3(0.13f)),
            bulb, false,
            isOn ? glm::vec3(1.0f, 0.45f, 0.08f) : glm::vec3(0.0f),
            isOn ? 2.5f : 0.0f,
            16.0f
        );
    }

    void FramebufferSizeCallback(GLFWwindow*, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void MouseCallback(GLFWwindow*, double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastMouseX = static_cast<float>(xpos);
            lastMouseY = static_cast<float>(ypos);
            firstMouse = false;
        }

        const float xOffset = static_cast<float>(xpos) - lastMouseX;
        const float yOffset = lastMouseY - static_cast<float>(ypos);

        lastMouseX = static_cast<float>(xpos);
        lastMouseY = static_cast<float>(ypos);

        camera.ProcessMouseMovement(xOffset, yOffset);
    }

    void ScrollCallback(GLFWwindow*, double, double yOffset)
    {
        camera.ProcessMouseScroll(static_cast<float>(yOffset));
    }

    void ProcessInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        const bool forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
        const bool backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
        const bool left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
        const bool right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

        camera.ProcessKeyboard(forward, backward, left, right, deltaTime);

        static bool previousOne = false;
        static bool previousTwo = false;

        const bool oneDown = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
        const bool twoDown = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;

        if (oneDown && !previousOne)
            lamp1On = !lamp1On;

        if (twoDown && !previousTwo)
            lamp2On = !lamp2On;

        previousOne = oneDown;
        previousTwo = twoDown;
    }
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "OpenGL Room - Week 5 Demo",
        nullptr,
        nullptr
    );

    if (!window)
    {
        std::cerr << "Failed to create GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD.\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    if (shader.ID == 0)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    std::unique_ptr<Mesh> cube(CreateCube());
    std::unique_ptr<Mesh> cylinder(CreateCylinder(0.5f, 1.0f, 32));
    std::unique_ptr<Mesh> frustum(CreateFrustum(0.5f, 0.25f, 1.0f, 32));
    std::unique_ptr<Mesh> sphere(CreateSphere(0.5f, 24, 16));

    shader.Use();
    shader.SetVec3("ambientLight", glm::vec3(0.055f, 0.065f, 0.08f));

    while (!glfwWindowShouldClose(window))
    {
        const float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ProcessInput(window);

        glClearColor(0.035f, 0.045f, 0.065f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int framebufferWidth = 0;
        int framebufferHeight = 0;
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

        const float aspect =
            framebufferHeight > 0
                ? static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight)
                : 1.0f;

        const glm::mat4 projection = glm::perspective(
            glm::radians(camera.GetZoom()),
            aspect,
            0.1f,
            100.0f
        );

        const glm::mat4 view = camera.GetViewMatrix();

        shader.Use();
        shader.SetMat4("view", view);
        shader.SetMat4("projection", projection);
        shader.SetVec3("viewPos", camera.Position);

        const glm::vec3 lamp1Position(-1.25f, 2.10f, -1.45f);
        const glm::vec3 lamp2Position( 1.25f, 2.10f, -1.45f);

        shader.SetVec3("lights[0].position", lamp1Position);
        shader.SetVec3("lights[0].color", glm::vec3(1.0f, 0.45f, 0.12f));
        shader.SetFloat("lights[0].intensity", lamp1On ? 3.2f : 0.0f);

        shader.SetVec3("lights[1].position", lamp2Position);
        shader.SetVec3("lights[1].color", glm::vec3(1.0f, 0.72f, 0.30f));
        shader.SetFloat("lights[1].intensity", lamp2On ? 2.8f : 0.0f);

        // Room
        DrawObject(
            *cube, shader,
            Transform(glm::vec3(0.0f, -0.08f, 0.0f), glm::vec3(8.0f, 0.16f, 8.0f)),
            glm::vec3(0.18f, 0.19f, 0.22f), true
        );

        DrawObject(
            *cube, shader,
            Transform(glm::vec3(0.0f, 2.0f, -4.0f), glm::vec3(8.0f, 4.0f, 0.16f)),
            glm::vec3(0.34f, 0.31f, 0.27f)
        );

        DrawObject(
            *cube, shader,
            Transform(glm::vec3(-4.0f, 2.0f, 0.0f), glm::vec3(0.16f, 4.0f, 8.0f)),
            glm::vec3(0.29f, 0.31f, 0.35f)
        );

        DrawObject(
            *cube, shader,
            Transform(glm::vec3(4.0f, 2.0f, 0.0f), glm::vec3(0.16f, 4.0f, 8.0f)),
            glm::vec3(0.29f, 0.31f, 0.35f)
        );

        DrawObject(
            *cube, shader,
            Transform(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(8.0f, 0.16f, 8.0f)),
            glm::vec3(0.22f, 0.23f, 0.26f)
        );

        // Rug
        DrawObject(
            *cube, shader,
            Transform(glm::vec3(0.0f, 0.025f, 1.0f), glm::vec3(5.5f, 0.05f, 3.6f)),
            glm::vec3(0.14f, 0.12f, 0.12f), true
        );

        // Table top and legs
        const glm::vec3 wood(0.25f, 0.11f, 0.045f);

        DrawObject(
            *cube, shader,
            Transform(glm::vec3(0.0f, 1.02f, -1.45f), glm::vec3(4.4f, 0.22f, 2.5f)),
            wood, false, glm::vec3(0.01f, 0.003f, 0.001f), 0.0f, 24.0f
        );

        for (float x : {-1.9f, 1.9f})
        {
            for (float z : {-2.4f, -0.5f})
            {
                DrawObject(
                    *cube, shader,
                    Transform(glm::vec3(x, 0.5f, z), glm::vec3(0.22f, 1.0f, 0.22f)),
                    wood
                );
            }
        }

        // A small cabinet on the back wall.
        DrawObject(
            *cube, shader,
            Transform(glm::vec3(-2.6f, 1.1f, -3.65f), glm::vec3(1.5f, 2.2f, 0.5f)),
            glm::vec3(0.19f, 0.20f, 0.22f)
        );

        DrawObject(
            *cube, shader,
            Transform(glm::vec3(-2.6f, 1.1f, -3.34f), glm::vec3(1.2f, 1.9f, 0.04f)),
            glm::vec3(0.07f, 0.09f, 0.12f)
        );

        DrawLamp(*cylinder, *frustum, *sphere, shader, -1.25f, -1.45f, lamp1On);
        DrawLamp(*cylinder, *frustum, *sphere, shader, 1.25f, -1.45f, lamp2On);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
