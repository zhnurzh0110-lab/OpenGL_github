# Laser Vault Heist

Real-time 3D stealth game for **Computer Graphics Fundamentals** (final project, topic 12).
You are a thief crossing a vault guarded by moving lasers and rotating guard spotlights. Reach the safe,
watch the security monitor, and slide under the last laser while the safe door swings open.

C++17 · OpenGL 3.3 core · GLFW · GLAD · GLM · Dear ImGui. No fixed-function calls, no engines.

## Team

| Member (surname, name, group) | Who did what |
|---|---|
| _TODO_ | _e.g. rendering pipeline, shaders, textures_ |
| _TODO_ | _e.g. level design, lasers, collision_ |
| _TODO_ | _e.g. camera, UI, CCTV, report_ |

## Build steps (same on Windows / Linux / macOS)

Requirements: CMake >= 3.16, a C++17 compiler, Git, Python-free. An OpenGL 3.3 capable GPU.

1. **GLAD (one-time).** Generate it at <https://glad.dav1d.de> with *Language: C/C++, API gl = Version 3.3,
   Profile: Core, Generate a loader: ON*. Unzip into `external/glad` so these exist:
   `external/glad/include/glad/glad.h`, `external/glad/include/KHR/khrplatform.h`, `external/glad/src/glad.c`.
   (Use the same GLAD files as in the course assignments.)
2. **GLFW, GLM, Dear ImGui.** If `external/glfw`, `external/glm`, `external/imgui` exist (vendored copies), they are
   used. Otherwise CMake downloads them at configure time (GLFW 3.4, GLM 1.0.1, ImGui 1.90.9).
   *If the grading machine has no internet, vendor them first:*
   ```
   git clone --depth 1 --branch 3.4    https://github.com/glfw/glfw.git        external/glfw
   git clone --depth 1 --branch 1.0.1  https://github.com/g-truc/glm.git       external/glm
   git clone --depth 1 --branch v1.90.9 https://github.com/ocornut/imgui.git   external/imgui
   ```
   (and delete their `.git` folders before committing).
3. **Configure and build**
   ```
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build --config Release
   ```
4. **Run** from anywhere (shaders are copied next to the executable and also found via the source tree):
   - Linux/macOS: `./build/LaserVaultHeist`
   - Windows (Visual Studio generator): `build\Release\LaserVaultHeist.exe`

## Controls

| Key | Action |
|---|---|
| W A S D | Walk (frame-rate independent) |
| Mouse | Look (click the window to capture, **Esc** to release) |
| Shift | Run |
| **C** or Ctrl (hold) | Crouch: smoothly lowers the camera and your hit-box |
| **E** | Interact: wall panel (disable rotating lasers) / crack the safe |
| **L** | Toggle vault lights |
| **V** | Switch CCTV camera shown on the security monitor |
| **R** | Restart |
| **H** | Show/hide the on-screen help |
| **G** | God mode (ignore lasers, for demos and debugging) |
| **F2** | Toggle VSync (to measure uncapped FPS) |

Goal: cross the laser room (crouch under high beams, time the gaps in low ones), collect the 5 money piles
(optional), and when you reach the safe press **E** to grab the diamond. Touching a laser turns every light red
and sends you back to the entrance.

## Features and where to find them (R1–R8)

| Req | Feature | Where |
|---|---|---|
| R1 | Core profile 3.3, VAO/VBO/EBO, own GLSL, no fixed function | `src/Mesh.cpp`, `shaders/phong.*`, `src/main.cpp` |
| R2 | 20+ objects: room, columns, pillars, 5 pedestals/money piles, safe, lasers, guards, panel, monitor, CCTV cameras | `src/Level.cpp` |
| R3 | Own model/view/projection matrices (GLM), scene graph with parents: safe → hinge → door → wheel; guard mount → arm → lamp head; laser pivot → core/halo | `src/SceneNode.cpp`, `src/Level.cpp`, `src/Camera.cpp` |
| R4 | Per-fragment Blinn-Phong (ambient + diffuse + specular), 6 lights: 3 ceiling points (toggle with **L**), 2 rotating spotlights (move), safe glow light | `shaders/phong.frag`, `Game::updateLights` |
| R5 | Procedural mip-mapped textures (floor tiles, concrete, brushed metal, banknote), box-projected world UVs, procedural CCTV screen effect (scanlines, vignette, grain) | `src/Texture.cpp`, `shaders/phong.frag` |
| R6 | FPS camera + crouch; interactions: toggle lights, wall panel, CCTV switch, crack safe, collect money, restart | `src/Game.cpp` |
| R7 | FPS counter in window title and in the ImGui HUD | `src/main.cpp`, `Game::drawUI` |
| R8 | CMake build described above | `CMakeLists.txt` |

**Core topic features:** lasers are thin glowing boxes (slide, sweep, rotate, static); touching one is detected with a
point-vs-box test in the laser's local space (the player is a vertical segment, so ducking under a high beam
works); an alarm turns every light red; walking and crouching camera.

**Stretch goals claimed:** (1) rotating guard spotlights (real spot lights driven by the scene graph),
(2) security monitor showing another camera via render-to-texture (FBO), (3) win condition at the safe.
Wow moment: the safe unlocks as you cross the last beam; slide under it while the door swings open.

## Project layout

```
src/Shader, Mesh, Texture, Framebuffer, Camera, SceneNode, Laser  (small, single-purpose modules)
src/Level.cpp      scene construction        src/Game.cpp   logic, render passes, HUD
shaders/           phong.vert, phong.frag
CREDITS.md         third-party libraries and sources
docs/              figures and software-rendered screenshots used in the report
tools/headless_render/  optional headless test that produced the screenshots
```

## Performance (fill in on the real test machine)

| Machine (CPU / GPU / OS) | Resolution | VSync | Avg FPS | Min FPS |
|---|---|---|---|---|
| _TODO_ | 1280x720 | off | | |

## AI Use note

_The team must complete and keep this honest._ The initial code skeleton of this project was produced with an
AI assistant (Claude, Anthropic): project structure, GLSL shaders, procedural textures, level layout, and this README.
Team changes: _TODO – describe exactly what you modified, tuned, debugged or rewrote, and by whom._
The report screenshots in `docs/screenshots/` were produced with the optional headless harness in `tools/headless_render/` (software rendering, no HUD). Take your own in-game captures with the FPS counter visible.
Every team member must be able to explain every file in the oral defense.
