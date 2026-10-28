#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Material.h"

class Mesh;
class Shader;

// Scene-graph node. world = parent.world * T * Ry * Rx * Rz * S.
// Convention used by the level: "pivot" nodes have scale 1 and no mesh and carry the
// motion; "geometry" nodes are leaves that carry a mesh and a size (scale).
class SceneNode {
public:
    explicit SceneNode(std::string name);

    std::string name;
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};   // degrees (x = pitch, y = yaw, z = roll)
    glm::vec3 scale{1.0f};
    const Mesh* mesh = nullptr;
    Material material;
    bool visible = true;
    bool hideInCCTV = false;    // skipped while rendering the security-camera view
    SceneNode* parent = nullptr;
    std::vector<std::unique_ptr<SceneNode>> children;

    SceneNode* addChild(const std::string& childName);
    glm::mat4 local() const;
    glm::mat4 world() const;

    // glowPass == false draws opaque nodes, true draws additive "glow" nodes.
    void render(Shader& shader, bool glowPass, bool cctv, const glm::mat4& parentWorld = glm::mat4(1.0f)) const;
};
