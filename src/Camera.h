#pragma once
#include <glm/glm.hpp>

// First-person camera. `feet` is the player's position on the floor, the eye is
// `eyeHeight` above it; crouching smoothly lowers the eye (frame-rate independent).
class Camera {
public:
    glm::vec3 feet{0.0f, 0.0f, 8.6f};
    float yaw   = -90.0f;     // degrees, -90 looks down -Z
    float pitch = 0.0f;
    float eyeHeight = 1.65f;
    float fov = 70.0f;

    static constexpr float kStandEye  = 1.65f;
    static constexpr float kCrouchEye = 0.85f;

    glm::vec3 eye() const { return feet + glm::vec3(0.0f, eyeHeight, 0.0f); }
    glm::vec3 front() const;
    glm::vec3 flatForward() const;
    glm::vec3 flatRight() const;
    glm::mat4 view() const;
    glm::mat4 projection(float aspect) const;
    float bodyHeight() const { return eyeHeight + 0.05f; }   // top of the player's collision segment

    void look(float dx, float dy, float sensitivity = 0.09f);
    void updateCrouch(bool crouching, float dt);
};
