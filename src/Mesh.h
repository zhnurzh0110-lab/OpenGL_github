#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

// Static indexed geometry living in a VAO + VBO + EBO.
class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void draw() const;

    // Unit primitives centred on the origin (scale them with the node transform).
    static std::unique_ptr<Mesh> cube();                       // 1 x 1 x 1
    static std::unique_ptr<Mesh> quad();                       // 1 x 1 in XY, facing +Z
    static std::unique_ptr<Mesh> cylinder(int segments = 32);  // radius 0.5, height 1, axis Y
    static std::unique_ptr<Mesh> sphere(int stacks = 24, int sectors = 32); // diameter 1

private:
    unsigned int vao_ = 0, vbo_ = 0, ebo_ = 0;
    int indexCount_ = 0;
};
