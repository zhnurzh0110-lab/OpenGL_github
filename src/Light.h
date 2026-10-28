#pragma once
#include <glm/glm.hpp>

struct Light {
    int   type = 1;                 // 1 = point, 2 = spot
    glm::vec3 pos{0.0f};
    glm::vec3 dir{0.0f, -1.0f, 0.0f};
    glm::vec3 color{1.0f};
    float intensity = 1.0f;
    float innerCos  = 0.97f;
    float outerCos  = 0.90f;
    bool  enabled   = true;
};
