# Laser Vault Heist

A real-time, first-person stealth game in C++17 and OpenGL 3.3 core profile.
Cross the vault, dodge moving lasers and guard spotlights, watch the CCTV monitor, and crack the safe.

![Screenshot](docs/screenshot.png)
<img width="1280" height="720" alt="fig05_rotating_lasers" src="https://github.com/user-attachments/assets/50f001cd-8067-4099-8119-5e62eb507a7e" />


## Features

- Blinn-Phong per-fragment lighting with six lights (two moving spotlights)
- Scene graph with parent-child motion (safe door, guard arms, laser pivots)
- Procedural, mipmapped textures (no external assets)
- CCTV monitor rendered from a second camera into a framebuffer
- Moving lasers (static, sliding, sweeping, rotating) with local-space collision
- First-person camera with walking, running and crouching

## Requirements

- C++17 compiler
- CMake 3.15 or newer
- GPU with OpenGL 3.3 support
- Internet access on the first configure (libraries are fetched by CMake)

Libraries: GLFW, GLAD, GLM, Dear ImGui.

## Build and Run

```bash
git clone <your-repo-url>
cd <your-repo-folder>
cmake -S . -B build
cmake --build build --config Release
```

Run the executable from the project root so the `shaders/` folder is found.

## Controls

| Key | Action |
|---|---|
| W A S D | Move |
| Shift | Run |
| C / Ctrl | Crouch |
| Mouse | Look |
| E | Interact (wall panel, safe) |
| L | Toggle ceiling lights |
| V | Switch CCTV camera |
| R | Restart |
| H | Hide/show help panel |
| G | God mode |
| F2 | VSync |
| Esc | Free the mouse |

**Goal:** cross the laser room, open the safe, and press E.

## Project Structure

```
src/        Shader, Mesh, Texture, Framebuffer, Camera, SceneNode, Laser, Level, Game
shaders/    GLSL 330 vertex and fragment shaders
docs/       Report and screenshots
CMakeLists.txt
```

## Team

| Name | Group | Role |
|---|---|---|
| Zhunis Nurzhigit | [SE-2421] | [developer] |

Course: Computer Graphics Fundamentals. Instructor: Nazira Shaimerdenova.

## AI Use

Claude (Anthropic) was used for the initial code skeleton (module structure, GLSL shaders, procedural textures, level layout), a draft of the report, and the final clean-up of the report and this README.

The team ran the game, took the screenshots, read the FPS values and recorded the test results.

[Describe what you personally modified, debugged or rewrote.]

This section matches the AI Use Note in the report.

## Credits

- [LearnOpenGL](https://learnopengl.com) for Blinn-Phong and framebuffer concepts
- [GLFW](https://www.glfw.org), [GLAD](https://glad.dav1d.de), [GLM](https://github.com/g-truc/glm), [Dear ImGui](https://github.com/ocornut/imgui)

All geometry and textures are generated in code, so no third-party assets are used.
