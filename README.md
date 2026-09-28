# OpenGLRoom

Interactive 3D room interior with lamps, built with modern shader-based OpenGL.

## Project requirements covered

- Real 3D room scene (not a single triangle)
- Modern OpenGL 3.3 Core Profile
- Vertex and fragment shaders (GLSL)
- Model / View / Projection transformation pipeline
- Camera support
- Lighting / shading model
- Texture/material support (assets can be placed in `textures/`)
- Keyboard and mouse interaction
- CMake build configuration
- Source-controlled third-party headers and GLFW library

## Technology

- C++20
- OpenGL 3.3 Core
- GLFW 3.5.1
- GLAD (OpenGL 3.3 core loader)
- GLM can be added to `Libraries/include` when the camera/scene implementation needs it

## Repository structure

```text
OpenGLRoom/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── Libraries/
│   ├── include/
│   │   ├── GLFW/
│   │   ├── glad/
│   │   └── KHR/
│   └── lib/
│       └── glfw3.lib
├── src/
│   ├── Main.cpp
│   ├── Camera.cpp
│   ├── Camera.h
│   ├── Model.cpp
│   ├── Model.h
│   ├── Shader.cpp
│   ├── Shader.h
│   └── glad.c
├── shaders/
├── textures/
├── models/
├── screenshots/
└── report/
```

Generated files such as `.vs/`, `x64/`, `Debug/`, `Release/`, `.obj`, `.pdb`, `.exe`, CMake cache/build folders, and Visual Studio user files are intentionally ignored by Git.

## Build on Windows

### Requirements

- Windows 10/11
- Visual Studio 2022 with **Desktop development with C++**
- CMake 3.20 or newer

### Configure

From the repository root:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

### Build

```powershell
cmake --build build --config Debug
```

### Run

```powershell
.\build\Debug\OpenGLRoom.exe
```

The current repository contains the starter OpenGL window. Team members can now add the room, shaders, camera, lighting, textures, and interaction on top of this clean build setup.

## Git workflow for the two-person team

Use feature branches instead of committing directly to `main`:

```bash
git checkout -b feature/rendering
git add .
git commit -m "Implement lighting and shaders"
git push -u origin feature/rendering
```

and for the second person:

```bash
git checkout -b feature/scene

git add .
git commit -m "Build room scene and camera"
git push -u origin feature/scene
```

Merge the feature branches into `main` through GitHub Pull Requests.

## Before submission

1. Clone the repository into a different folder or another computer.
2. Configure and build it using the commands above.
3. Verify that no absolute paths such as `C:\Users\...` are required.
4. Verify that all shaders, textures, and models used by the program are committed.
5. Do not commit generated Visual Studio/CMake build output.
