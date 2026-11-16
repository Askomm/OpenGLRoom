# OpenGLRoom — Room Interior with Lamps

An interactive 3D room built with **C++20 and OpenGL 3.3 Core** for the Computer Graphics Fundamentals final project, **Topic 16: Room interior with lamps**.

Explore a furnished study and bedroom, switch individual lamps on and off, move the ceiling pendant, and compare daylight with night lighting. The project demonstrates programmable shaders, geometric transformations, camera control, Phong lighting, shadow mapping, and procedural surface patterns.

| Team member | Group |
| --- | --- |
| Azamat Kuanyshpayev | IT-2514 |
| Arsen Galym | IT-2510 |

## Features

- **Furnished interior:** a writing desk, two desk lamps, a chair, cabinet, books, vase, rug, and bed.
- **Three independent lamps:** warm and cool desk lights plus a movable ceiling pendant.
- **Day and night modes:** different window textures and ambient lighting, with direct sunlight available during the day.
- **Dynamic shadows:** separate 1024 × 1024 depth maps for the pendant and sunlight, filtered using 3 × 3 percentage-closer filtering (PCF).
- **Interactive pendant:** manual horizontal movement or automatic travel, with the light and its shadow moving together.
- **Material variation:** procedural wood and woven-fabric patterns, material-dependent specular highlights, and emissive bulbs.
- **First-person navigation:** mouse look, scroll-wheel zoom, and basic collision checks against furniture and room boundaries.
- **Exposure control:** adjustable scene brightness with tone mapping and gamma correction.
- **On-screen interface:** light/shadow indicators and an F1 control guide.

## Requirements

The supplied build configuration targets **Windows x64 with MSVC**.

| Requirement | Details |
| --- | --- |
| Operating system | Windows 10 or 11, 64-bit |
| Compiler | Visual Studio 2022 or 2026 with **Desktop development with C++** |
| Build system | CMake 3.20 or later; the installed version must support your Visual Studio generator |
| Graphics | GPU and driver supporting OpenGL 3.3 Core |
| Git | Optional; needed only when cloning instead of downloading a ZIP |

GLFW 3.5.1, GLAD, and GLM are included in the repository. No separate installation of those libraries is required for the supplied Windows build.

> The bundled GLFW static library uses the MSVC debug DLL runtime. CMake selects the matching runtime in every configuration. Use the Debug build below for development and demonstration; changing only the configuration to Release does not remove this debug-runtime dependency.

## Build and run

### 1. Get the project

```bat
git clone https://github.com/Askomm/OpenGLRoom.git
cd OpenGLRoom
```

Alternatively, select **Code → Download ZIP** on GitHub, extract the complete archive, and open a terminal in the folder containing `CMakeLists.txt`.

### 2. Configure and build

Open an **x64 Native Tools Command Prompt** for your installed Visual Studio version.

**Visual Studio 2026:**

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Debug
```

**Visual Studio 2022:**

```bat
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
```

Choose one generator. If switching Visual Studio versions after configuring, use a new build directory, such as `build-vs2022`, in both commands.

### 3. Start the application

From the repository root:

```bat
.\build\Debug\OpenGLRoom.exe
```

The application opens a **1280 × 720** window in night mode with all three lamps and shadows enabled. Mouse movement controls the camera immediately.

CMake copies `shaders/`, `textures/`, and `models/` beside the executable after a build. Keep these directories together when moving the program. The application checks for assets beside its executable.

## Controls

| Input | Action |
| --- | --- |
| **W / S** | Move forward / backward at a fixed eye height |
| **A / D** | Move left / right |
| **Mouse** | Look around |
| **Mouse wheel** | Adjust field of view between 25° and 75° |
| **1** | Toggle the warm desk lamp |
| **2** | Toggle the cool desk lamp |
| **3** | Toggle the ceiling pendant light |
| **4** | Toggle direct sunlight; visible only in day mode |
| **N** | Switch between day and night |
| **H** | Toggle pendant and sunlight shadow mapping |
| **J / L** | Hold to move the pendant left / right |
| **Space** | Start or pause automatic pendant travel |
| **- / =** | Hold to decrease / increase exposure, from 0.25 to 3.0 |
| **R** | Reset the camera, lights, shadows, exposure, and pendant to the initial night scene |
| **F1** | Show or hide the control guide |
| **Esc** | Close the application |

Manual pendant movement takes priority while **J** or **L** is held. If automatic travel is enabled, it resumes when the key is released.

## Suggested demonstration

1. Press **R** to start from the default night view. Use **W/A/S/D**, the mouse, and the wheel to show navigation.
2. Toggle **1**, **2**, and **3** individually to compare each light's contribution.
3. Press **R**, then **1** and **2**, leaving only the pendant enabled. Toggle **H** and observe the shadows around the desk and floor.
4. Hold **J** or **L**, then press **Space**, to demonstrate moving illumination and shadows.
5. Press **R**, then **N**. Turn off lamps **1**, **2**, and **3** to isolate the daylight contribution.
6. Toggle **4** to compare direct sunlight with daylight ambient illumination. Toggle **H** to compare furniture shadows.
7. Adjust exposure with **- / =**, show the guide with **F1**, and finish with **R**.

## How rendering works

The scene uses procedural cube, cylinder, frustum, and sphere meshes stored in vertex and index buffers. Individual objects are positioned, rotated, and scaled with model matrices. The camera supplies the view matrix, and a perspective projection follows the window's framebuffer aspect ratio.

Each frame performs the following work:

1. **Input and animation:** update the camera, light switches, exposure, and pendant position.
2. **Pendant depth pass:** when the pendant and shadows are enabled, render scene depth from the pendant's perspective into its shadow map.
3. **Sunlight depth pass:** during the day, when sunlight and shadows are enabled, render a separate orthographic depth map.
4. **Scene pass:** render from the camera using ambient, diffuse, and specular lighting, attenuation, material patterns, emission, and shadow visibility.
5. **Interface pass:** draw the status panel or help guide over the scene.

The vertex shader transforms normals using the inverse-transpose model matrix. The fragment shader evaluates three positional lights with Phong reflection; the pendant also has a downward angular falloff. Directional sunlight uses parallel rays and an analytical window-opening test, including the two interior window mullions.

For lit geometry, exposure is applied before Reinhard-style tone mapping, followed by gamma correction. The window illustration and interface are rendered separately from this lighting calculation.

## Project structure

| Path | Purpose |
| --- | --- |
| [CMakeLists.txt](CMakeLists.txt) | Build configuration, bundled dependencies, and asset copying |
| [src/Main.cpp](src/Main.cpp) | OpenGL setup, procedural meshes, input, lighting, render passes, and interface |
| [src/Interior.h](src/Interior.h) | Room and furniture placement |
| [src/SceneLayout.h](src/SceneLayout.h) | Shared desk offset used by geometry, lighting, and collision |
| [src/Camera.cpp](src/Camera.cpp), [src/Camera.h](src/Camera.h) | Camera movement, mouse look, zoom, and collision checks |
| [src/Shader.cpp](src/Shader.cpp), [src/Shader.h](src/Shader.h) | GLSL loading, compilation, linking, diagnostics, and uniforms |
| [src/glad.c](src/glad.c) | OpenGL function loading |
| [shaders/](shaders/) | Scene, shadow-depth, and interface shaders |
| [textures/](textures/) | PPM window images, interface panels, and font license |
| [Libraries/](Libraries/) | Bundled GLFW, GLAD, and GLM dependencies |
| [report/](report/) | Report, presentation, and demonstration video |

`src/Model.cpp`, `src/Model.h`, and `models/` are placeholders; the current scene does not require imported model files. `screenshots/` is reserved for screenshots and currently contains no preview images.

## Project materials

- [Final report](report/Final%20Report.docx)
- [Presentation](report/CompGraphPresentation.pptx)
- [Demonstration video](report/video.mp4)

## Troubleshooting

| Problem | What to check |
| --- | --- |
| `cmake` is not recognized | Install CMake or enable Visual Studio's CMake tools, then reopen the developer terminal. |
| Visual Studio generator is unavailable | Use the generator matching your installation. Run `cmake --help` to see the generators supported by your CMake version. |
| Generator differs from the existing cache | Configure into a new build directory and use that same directory when building. |
| Missing GLFW library or headers | Confirm the complete repository was extracted, including `Libraries/lib/glfw3.lib` and `Libraries/include/GLFW/glfw3.h`. |
| Runtime mismatch or missing debug-runtime DLL | Use the x64 MSVC toolchain with the C++ workload installed and keep the supplied runtime setting. Standalone distribution requires rebuilding GLFW and the application with a compatible release runtime. |
| `Could not open shader file` or `Cannot load room textures` | Check that the copied `shaders/` and `textures/` folders are beside the executable. Run from a terminal to read diagnostics. |
| GLFW window creation or GLAD initialization fails | Check that the graphics driver and active environment support OpenGL 3.3 Core. |
| Sunlight appears to have no effect | Press **N** to enter day mode, then **4** if sunlight is disabled. Turn off the three lamps to make the difference easier to see. |

## Scope and limitations

- The current CMake configuration intentionally rejects non-Windows builds because it bundles a Windows GLFW library. Linux or macOS requires adapting dependency discovery and platform linking.
- Only the pendant and sunlight cast mapped shadows. The two desk lamps contribute lighting without individual shadow maps.
- Collision uses simple horizontal bounds at a fixed eye height; there is no physics simulation.
- Geometry and furniture placement are defined in code; there is no model importer or scene editor.
- This is a real-time educational renderer using Phong shading, not a physically based renderer.
- Historical FPS figures from earlier revisions are not benchmarks for the current bed-and-daylight scene. Measure performance on the actual demonstration PC.

## Credits

The project combines rendering, shaders, and lighting work with scene construction, camera interaction, and interface work. Integration and presentation are team responsibilities.

Room geometry, landscape images, and surface patterns were produced for this project. The interface uses **DejaVu Sans**; its license is included in [textures/FONT-LICENSE.txt](textures/FONT-LICENSE.txt). Third-party components retain their respective licenses.

