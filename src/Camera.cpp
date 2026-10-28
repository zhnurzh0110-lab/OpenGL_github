#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

glm::vec3 Camera::front() const
{
    float y = glm::radians(yaw), p = glm::radians(pitch);
    return glm::normalize(glm::vec3(std::cos(y) * std::cos(p), std::sin(p), std::sin(y) * std::cos(p)));
}

glm::vec3 Camera::flatForward() const
{
    float y = glm::radians(yaw);
    return glm::normalize(glm::vec3(std::cos(y), 0.0f, std::sin(y)));
}

glm::vec3 Camera::flatRight() const
{
    return glm::normalize(glm::cross(flatForward(), glm::vec3(0, 1, 0)));
}

glm::mat4 Camera::view() const
{
    return glm::lookAt(eye(), eye() + front(), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::projection(float aspect) const
{
    return glm::perspective(glm::radians(fov), aspect, 0.05f, 100.0f);
}

void Camera::look(float dx, float dy, float sensitivity)
{
    yaw += dx * sensitivity;
    pitch = std::clamp(pitch - dy * sensitivity, -89.0f, 89.0f);
}

void Camera::updateCrouch(bool crouching, float dt)
{
    float target = crouching ? kCrouchEye : kStandEye;
    eyeHeight += (target - eyeHeight) * (1.0f - std::exp(-10.0f * dt));
}
