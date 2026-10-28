#include "SceneNode.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <glm/gtc/matrix_transform.hpp>

SceneNode::SceneNode(std::string n) : name(std::move(n)) {}

SceneNode* SceneNode::addChild(const std::string& childName)
{
    children.push_back(std::make_unique<SceneNode>(childName));
    SceneNode* c = children.back().get();
    c->parent = this;
    return c;
}

glm::mat4 SceneNode::local() const
{
    glm::mat4 m = glm::translate(glm::mat4(1.0f), position);
    m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    return glm::scale(m, scale);
}

glm::mat4 SceneNode::world() const
{
    return parent ? parent->world() * local() : local();
}

void SceneNode::render(Shader& sh, bool glowPass, bool cctv, const glm::mat4& parentWorld) const
{
    if (!visible || (cctv && hideInCCTV)) return;

    glm::mat4 w = parentWorld * local();

    if (mesh && ((material.mode == ShadeMode::Glow) == glowPass)) {
        sh.setMat4("uModel", w);
        sh.setMat3("uNormalMat", glm::mat3(glm::transpose(glm::inverse(w))));
        sh.setInt("uMode", static_cast<int>(material.mode));
        sh.setVec3("uTint", material.tint);
        sh.setFloat("uSpecular", material.specular);
        sh.setFloat("uShininess", material.shininess);
        sh.setVec3("uEmissive", material.emissive);
        sh.setFloat("uAlpha", material.alpha);
        sh.setInt("uWorldUV", material.worldUV ? 1 : 0);
        sh.setFloat("uUVScale", material.uvScale);
        if (material.tex) material.tex->bind(0);
        mesh->draw();
    }
    for (const auto& c : children) c->render(sh, glowPass, cctv, w);
}
