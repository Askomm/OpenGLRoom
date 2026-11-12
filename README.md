# Topic 16 – Room interior with lamps

Azamat Kuanyshpayev (IT-2514) and Arsen Galym (IT-2510). Final Computer Graphics Fundamentals walkthrough based on the team’s Week 5 commit d6688cc.

Two desk fixtures illuminate a furnished study in amber and cool blue. A ceiling pendant travels across the room, casting filtered shadows on the desk, chair and rug. A bed sits against the left wall. Day mode adds a fourth source, directional sunlight through the window. The initial 1280 × 720 night view shows all three fixtures.

## Windows build and run

Use an x64 Native Tools Command Prompt for Visual Studio 2026 on Windows 10 or 11, with the Desktop development with C++ workload, CMake, and an OpenGL 3.3-capable driver. Run from the directory containing CMakeLists.txt:

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Debug
.\build\Debug\OpenGLRoom.exe
```

For Visual Studio 2022 use its matching generator, "Visual Studio 17 2022". Choose a CMake version that supports the generator installed on your PC. The project bundles GLAD, GLM and the x64 Windows GLFW 3.5.1 static library. The GLFW binary embeds the MSVC debug DLL runtime, so use the Visual Studio developer environment. CMake copies the shader and texture directories beside the executable. Keep them there when moving the program.

## Controls

| Input | Action |
| --- | --- |
| W / A / S / D | Walk at eye height. The desk, chair, cabinet, bed and room boundary block movement. |
| Mouse / wheel | Look / zoom, with a field of view from 25° to 75°. |
| 1 / 2 / 3 | Toggle the amber desk lamp, cool desk lamp and pendant independently. |
| H | Toggle pendant and sunlight shadow maps and depth comparisons. |
| N | Switch day / night, updating the window view, ambient light and sunlight. |
| 4 | Toggle direct sunlight during the day. |
| J / L | Move the pendant left / right. |
| Space | Start or pause automatic pendant travel. |
| − / = | Hold to decrease / increase exposure. |
| R | Restore the original camera, lights, exposure and stationary pendant and night mode. |
| F1 | Open or close the on-screen control guide. |
| Esc | Exit. |

## Best shot and live check

1. Press R. Compare the desk and rug with all lights on, then press 1 and 2 to isolate the pendant.
2. Press H twice while watching the shadow under the writing desk.
3. Hold J or L. The pendant bulb and its shadow should move together. Press Space to start automatic travel and again to pause.
4. Hold minus, then equal, to demonstrate tone mapping at different exposures. Press R to return to the default view.
5. Resize the window on the lab PC. The perspective should follow the new framebuffer aspect. Press F1 for the key guide.

## Sunlight and bed

Press N for day, then 1, 2 and 3 to isolate sunlight. Press 4 to compare the direct sun contribution while keeping daylight ambient light. Look at the desk, bed and floor; H toggles furniture shadows. Press R to restore the night view.

Sunlight is a fourth source with parallel rays in world space. It uses a 1024 × 1024 orthographic depth map with nine comparisons per fragment. A ray/window intersection admits direct light only through the opening and blocks the two interior mullions. Room walls and the opaque landscape panel are excluded from this depth pass because the analytical aperture supplies their visibility. Furniture and lamp meshes cast sun shadows. Night mode skips the sunlight pass. The sun uses the same Phong materials and exposure curve as the lamps.

The bed has a timber frame, mattress, woven cover and pillow. Its bounds participate in camera collision. The desk, chair, books and both desk lamps are shifted 0.8 scene units to the right, leaving about 0.94 units between the headboard and desk. Geometry, light positions and camera collision use the shared DeskOffsetX in src/SceneLayout.h.

## Code guide

| File | Role |
| --- | --- |
| src/Main.cpp | Creates the OpenGL context and procedural meshes, manages the pendant, renders depth and color passes, and draws the HUD. |
| src/Shader.cpp | Loads external GLSL, logs compile/link errors and sets uniforms. |
| shaders/shadow.vert and shadow.frag | Render depth from the pendant into a 1024 × 1024 texture. |
| shaders/vertex.glsl and fragment.glsl | Transform positions/normals, evaluate three forward Phong lights, filter the pendant shadow, apply procedural materials and tone-map the result. |
| src/Interior.h | Places furniture, walls, window, floor, chair, books and glazed vase. |
| src/Camera.cpp | Handles mouse look, scroll zoom, eye-height movement and obstacles. |
| textures/ and shaders/status.* | Dusk illustration, status/help panels and screen overlay. |
| report/ | Final report, presentation with speaker notes, and 60-second backup. |

## Verification

The report, slides, backup video and timing figures below describe the earlier three-fixture version. The bed/daylight revision adds a second shadow pass during daytime, so those timing figures are historical.

Both source stages were compiled and rendered using the same application scene code with scripted GLFW inputs and a Mesa OpenGL core context. The final build was timed on llvmpipe at 1280 × 720 after 15 warm-up frames. With the pendant shadow map it averaged 80.3 FPS, compared with 101.9 FPS without it (80 synchronized frames per run). The previous Stage 1 build was confirmed on Windows/MSVC. The bed/daylight revision was compiled and rendered with Mesa llvmpipe and scripted input; it still needs a Windows runtime check. The report lists the pixel checks, method, limitations, literature and asset credits.

## Contributions and credits

Person 1 owns the initialization, shader system, MVP, materials and lighting. Person 2 owns the furnished scene, geometry, textures, camera and interface. Both share final integration, tests, report, slides and defence. The report documents AI assistance. Geometry, landscape and patterns were produced for this project. HUD text uses DejaVu Sans; see textures/FONT-LICENSE.txt.
