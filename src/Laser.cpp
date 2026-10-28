#include "Laser.h"
#include <cmath>

void Laser::update(float t)
{
    float s = std::sin(speed * t + phase);
    switch (motion) {
    case LaserMotion::SlideX: pivot->position.x = base.x + amp * s; break;
    case LaserMotion::SweepY: pivot->position.y = base.y + amp * s; break;
    case LaserMotion::Rotate: pivot->rotation.y = glm::degrees(speed * t + phase); break;
    case LaserMotion::Static: break;
    }
    pivot->visible = active;
}

bool Laser::hits(const glm::vec3& feet, float height, float radius) const
{
    if (!active) return false;

    glm::mat4 w = pivot->world();
    float centreY = w[3].y;
    glm::vec3 local = glm::vec3(glm::inverse(w) * glm::vec4(feet.x, centreY, feet.z, 1.0f));

    if (std::abs(local.x) > half.x + radius) return false;
    if (std::abs(local.z) > half.z + radius) return false;

    const float margin = 0.03f;   // slightly fat beam so thin lasers are not skipped
    float lo = centreY - half.y - margin, hi = centreY + half.y + margin;
    return feet.y <= hi && feet.y + height >= lo;
}
