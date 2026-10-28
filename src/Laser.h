#pragma once
#include <glm/glm.hpp>
#include "SceneNode.h"

enum class LaserMotion { Static, SlideX, SweepY, Rotate };

// A laser beam is a thin box (local X is the beam direction) hanging under a pivot node.
struct Laser {
    SceneNode* pivot = nullptr;
    LaserMotion motion = LaserMotion::Static;
    glm::vec3 base{0.0f};            // rest position of the pivot
    glm::vec3 half{1.0f, 0.02f, 0.02f};   // half extents of the beam box (local space)
    float amp = 0.0f, speed = 1.0f, phase = 0.0f;
    int group = 0;                   // group 1 can be switched off at the wall panel
    bool active = true;

    void update(float t);

    // Point-vs-box test in the laser's local space. The player is a vertical segment
    // [feet, feet + height] with horizontal radius `radius`; we test the segment's XZ
    // position against the box footprint and its Y interval against the box height.
    bool hits(const glm::vec3& feet, float height, float radius) const;
};
