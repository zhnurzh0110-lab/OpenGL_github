// Headless test harness: runs the REAL game code (Game, Level, shaders) on Mesa's software
// OpenGL through EGL and saves frames. Not part of the delivered game.
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <dlfcn.h>
#include <glad/glad.h>
#define private public
#include "Game.h"
#undef private

typedef void* EGLDisplay; typedef void* EGLConfig; typedef void* EGLSurface; typedef void* EGLContext; typedef int EGLint; typedef unsigned EGLBoolean;
static void* lib;
#define EGLFN(ret,name,args) typedef ret (*PF_##name) args; static PF_##name name = nullptr;
EGLFN(EGLDisplay,eglGetDisplay,(void*)) EGLFN(EGLDisplay,eglGetPlatformDisplay,(unsigned,void*,const EGLint*))
EGLFN(EGLBoolean,eglInitialize,(EGLDisplay,EGLint*,EGLint*)) EGLFN(EGLBoolean,eglChooseConfig,(EGLDisplay,const EGLint*,EGLConfig*,EGLint,EGLint*))
EGLFN(EGLBoolean,eglBindAPI,(unsigned)) EGLFN(EGLSurface,eglCreatePbufferSurface,(EGLDisplay,EGLConfig,const EGLint*))
EGLFN(EGLContext,eglCreateContext,(EGLDisplay,EGLConfig,EGLContext,const EGLint*)) EGLFN(EGLBoolean,eglMakeCurrent,(EGLDisplay,EGLSurface,EGLSurface,EGLContext))
EGLFN(void*,eglGetProcAddress,(const char*)) EGLFN(EGLint,eglGetError,())

static void* loadGL(const char* n){ void* p = eglGetProcAddress(n); return p; }

static const int W = 1280, H = 720;

static void save(const char* path)
{
    std::vector<unsigned char> px(W * H * 4), flip(W * H * 3);
    glReadPixels(0, 0, W, H, GL_RGBA, GL_UNSIGNED_BYTE, px.data());
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            for (int c = 0; c < 3; ++c) flip[(y * W + x) * 3 + c] = px[((H - 1 - y) * W + x) * 4 + c];
    FILE* f = std::fopen(path, "wb");
    std::fprintf(f, "P6\n%d %d\n255\n", W, H);
    std::fwrite(flip.data(), 1, flip.size(), f);
    std::fclose(f);
    std::printf("saved %s\n", path);
}

struct Shot { const char* file; glm::vec3 feet; float yaw, pitch, eye; float t; bool lights; bool alarm; float door; };

int main(int argc, char** argv)
{
    const char* outdir = argc > 1 ? argv[1] : ".";
    lib = dlopen("libEGL.so.1", RTLD_NOW);
    if (!lib) { std::puts("no libEGL"); return 1; }
#define LOADE(n) n = (PF_##n)dlsym(lib, #n)
    LOADE(eglGetDisplay); LOADE(eglGetPlatformDisplay); LOADE(eglInitialize); LOADE(eglChooseConfig); LOADE(eglBindAPI);
    LOADE(eglCreatePbufferSurface); LOADE(eglCreateContext); LOADE(eglMakeCurrent); LOADE(eglGetProcAddress); LOADE(eglGetError);

    EGLDisplay dpy = eglGetDisplay(nullptr);
    EGLint maj, min;
    if (!dpy || !eglInitialize(dpy, &maj, &min)) {
        dpy = eglGetPlatformDisplay(0x31DD, nullptr, nullptr);          // EGL_PLATFORM_SURFACELESS_MESA
        if (!dpy || !eglInitialize(dpy, &maj, &min)) { std::puts("EGL init failed"); return 1; }
    }
    const EGLint cfgAttr[] = { 0x3033, 1, 0x3040, 8, 0x3024, 8, 0x3023, 8, 0x3022, 8, 0x3021, 8, 0x3025, 24, 0x3038 };
    EGLConfig cfg; EGLint n = 0;
    if (!eglChooseConfig(dpy, cfgAttr, &cfg, 1, &n) || n < 1) { std::puts("no EGL config"); return 1; }
    eglBindAPI(0x30A2);
    const EGLint sAttr[] = { 0x3057, W, 0x3056, H, 0x3038 };
    EGLSurface surf = eglCreatePbufferSurface(dpy, cfg, sAttr);
    const EGLint cAttr[] = { 0x3098, 3, 0x30FB, 3, 0x30FD, 1, 0x3038 };
    EGLContext ctx = eglCreateContext(dpy, cfg, nullptr, cAttr);
    if (!ctx || !eglMakeCurrent(dpy, surf, surf, ctx)) { std::printf("context failed 0x%x\n", eglGetError()); return 1; }
    if (!gladLoadGLLoader(loadGL)) std::puts("warning: some GL functions missing");

    Game g;
    g.init(nullptr);
    std::puts("Game initialised: shaders compiled, level built.");

    const Shot shots[] = {
      {"01_entrance.ppm",        {0,0,8.6f},   -90, -4, 1.65f, 1.0f, true,  false, 0},
      {"02_lights_off.ppm",      {0,0,8.6f},   -90, -4, 1.65f, 1.0f, false, false, 0},
      {"03_alarm.ppm",           {0,0,6.9f},   -90, -6, 1.65f, 0.1f, true,  true,  0},
      {"04_rotating_lasers.ppm", {0,0,3.9f},   -90, -8, 1.65f, 2.2f, true,  false, 0},
      {"05_crouch_last_laser.ppm",{0,0,-5.2f}, -90,  2, 0.85f, 3.0f, true,  false, 105},
      {"06_safe_open.ppm",       {0,0,-6.9f},  -90,  4, 1.65f, 3.0f, true,  false, 105},
      {"07_cctv_monitor.ppm",    {-4.3f,0,7.2f}, 180, 2, 1.65f, 1.4f, true,  false, 0},
      {"08_floor_closeup.ppm",   {-3.5f,0,-3.0f}, -60, -48, 1.65f, 1.0f, true, false, 0},
      {"09_zone_overview.ppm",   {0,0,9.2f},   -90, -22, 1.65f, 0.6f, true,  false, 0},
    };
    for (const Shot& s : shots) {
        g.god_ = true;
        g.lightsOn_ = s.lights;
        g.time_ = s.t;
        g.state_ = s.alarm ? GameState::Alarm : GameState::Playing;
        g.alarmTimer_ = 2.0f;
        g.doorAngle_ = s.door; g.doorTarget_ = s.door > 0 ? 1.0f : 0.0f;
        g.update(0.0001f);
        g.cam_.feet = s.feet; g.cam_.yaw = s.yaw; g.cam_.pitch = s.pitch; g.cam_.eyeHeight = s.eye;
        g.alarmLevel_ = s.alarm ? 0.95f : 0.0f;
        g.render(W, H);
        std::string path = std::string(outdir) + "/" + s.file;
        save(path.c_str());
    }
    unsigned err = glGetError();
    std::printf("glGetError after all frames: 0x%x\n", err);
    return 0;
}
