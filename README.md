# OpenGL Room

This is our OpenGL final project demo : a small 3D room with two lamps and simple furniture.

For the Week 5 demo, the main parts are already connected:

- A real 3D room scene
- Modern OpenGL 3.3 core profile
- Vertex and fragment shaders
- Model -> View -> Projection transformations
- Blinn-Phong style lighting with two point lights
- Emissive lamp bulbs
- A simple procedural checker pattern on the floor
- Camera movement with WASD
- Mouse look and scroll zoom
- `1` and `2` keys to turn each lamp on/off

## Controls

| Key | Action |
| --- | --- |
| W / S | Move forward / backward |
| A / D | Move left / right |
| Mouse | Look around |
| Mouse wheel | Zoom |
| 1 | Toggle left lamp |
| 2 | Toggle right lamp |
| Esc | Exit |

## Project structure

```text
OpenGLRoom/
├── CMakeLists.txt
├── README.md
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
│   ├── vertex.glsl
│   └── fragment.glsl
├── textures/
├── models/
├── Libraries/
└── report/
```

The current scene is made from simple procedural geometry, so extra model or texture files will be added later.

## Build

We are using Visual Studio 2026 (Visual Studio 18), CMake, GLFW, GLAD and GLM on Windows. The project uses C++20.

The easiest way is to open the project folder in Visual Studio 2026 and let CMake configure it.

From the project folder, the equivalent CMake command is:

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Debug
```

Then run:

```powershell
.\build\Debug\OpenGLRoom.exe
```

## Week 5 demo

The rendering side and the camera are connected now. The scene shows how the transformation pipeline, shader code, lighting and user input work together.

More models, materials and scene details will be added in the later weeks.
