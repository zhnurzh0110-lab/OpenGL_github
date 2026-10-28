# Headless render test (optional, not part of the game)

Used to produce the report screenshots in `docs/screenshots/` without a window. It compiles the real game
sources (everything in `src/` except `main.cpp`) against small shim headers (`inc/`: a minimal GLM, a GL loader
over EGL, no-op GLFW/ImGui) and renders scripted camera positions on Mesa's software rasteriser.
Because it is software rendering, it proves the shaders and scene work but says nothing about FPS, and the HUD is not drawn.

```
g++ -std=c++17 -O1 -I inc -I ../../src -DGLFW_INCLUDE_NONE -DSHADER_DIR='"../../shaders/"' \
    $(ls ../../src/*.cpp | grep -v main.cpp) harness.cpp imgui_stub.cpp -o harness -ldl
mkdir -p out && LIBGL_ALWAYS_SOFTWARE=1 MESA_GL_VERSION_OVERRIDE=3.3 MESA_GLSL_VERSION_OVERRIDE=330 xvfb-run -a ./harness out
```
Output is PPM files. Delete this folder if you do not want it in the submission.
