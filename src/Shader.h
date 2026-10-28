#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

// Compiles and links a GLSL vertex + fragment shader loaded from shaders/*.vert|frag
class Shader {
public:
    Shader(const std::string& vertName, const std::string& fragName);
    ~Shader();
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void use() const;
    void setInt(const std::string& n, int v) const;
    void setFloat(const std::string& n, float v) const;
    void setVec3(const std::string& n, const glm::vec3& v) const;
    void setMat3(const std::string& n, const glm::mat3& m) const;
    void setMat4(const std::string& n, const glm::mat4& m) const;

private:
    int location(const std::string& n) const;
    unsigned int id_ = 0;
    mutable std::unordered_map<std::string, int> cache_;
};
