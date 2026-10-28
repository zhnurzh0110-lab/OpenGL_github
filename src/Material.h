#pragma once
#include <glm/glm.hpp>

class Texture;

// Must match the MODE_* defines in shaders/phong.frag
enum class ShadeMode { Lit = 0, Emissive = 1, Screen = 2, Glow = 3, Laser = 4 };

struct Material {
    const Texture* tex = nullptr;
    glm::vec3 tint{1.0f};
    float specular  = 0.3f;
    float shininess = 32.0f;
    glm::vec3 emissive{0.0f};
    ShadeMode mode  = ShadeMode::Lit;
    bool  worldUV   = false;   // true: box-projected world-space UVs (no stretching on scaled cubes)
    float uvScale   = 1.0f;    // tiles per metre (worldUV) or UV multiplier (mesh UV)
    float alpha     = 1.0f;    // used by Glow mode
};
