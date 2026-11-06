# Topic 16 – Room interior with lamps

## Renderer contribution (Person 1)

This first-stage snapshot builds on the Week 5 scene. Two desk lights and a movable ceiling pendant use per-fragment Phong shading. The pendant renders a 1024 × 1024 depth texture for filtered shadows. Press H for a controlled comparison. The second changeset adds the finished interior and on-screen controls.

## Windows build and run

Use an x64 Native Tools Command Prompt for Visual Studio 2026 on Windows 10 or 11, with the Desktop development with C++ workload, CMake, and an OpenGL 3.3-capable driver. Run from the directory containing CMakeLists.txt:

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Debug
.\build\Debug\OpenGLRoom.exe
```

For Visual Studio 2022 use its matching generator, "Visual Studio 17 2022". Choose a CMake version that supports the generator installed on your PC. The project bundles GLAD, GLM and the x64 Windows GLFW 3.5.1 static library. The GLFW binary embeds the MSVC debug DLL runtime, so use the Visual Studio developer environment. CMake copies the shader and texture directories beside the executable. Keep them there when moving the program.

## Input

WASD moves the camera, mouse looks, and the wheel zooms. Keys 1, 2, and 3 toggle the desk fixtures and pendant respectively. J/L move the pendant; Space starts or pauses its automatic sweep. H toggles pendant shadows, minus/equal adjust exposure, and Esc exits.
