#include "Mesh.h"
#include <glad/glad.h>
#include <cmath>
#include <cstddef>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : indexCount_(static_cast<int>(indices.size()))
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}

void Mesh::draw() const
{
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, nullptr);
}

namespace {
// Adds one planar face: centre + n*half, spanned by tangents u, v (u x v == n  =>  CCW).
void addFace(std::vector<Vertex>& vs, std::vector<unsigned int>& is,
             glm::vec3 n, glm::vec3 u, glm::vec3 v, float offset)
{
    unsigned int base = static_cast<unsigned int>(vs.size());
    glm::vec3 c = n * offset;
    vs.push_back({ c - 0.5f * u - 0.5f * v, n, {0, 0} });
    vs.push_back({ c + 0.5f * u - 0.5f * v, n, {1, 0} });
    vs.push_back({ c + 0.5f * u + 0.5f * v, n, {1, 1} });
    vs.push_back({ c - 0.5f * u + 0.5f * v, n, {0, 1} });
    for (unsigned int i : { 0u, 1u, 2u, 0u, 2u, 3u }) is.push_back(base + i);
}
} // namespace

std::unique_ptr<Mesh> Mesh::cube()
{
    std::vector<Vertex> vs;
    std::vector<unsigned int> is;
    addFace(vs, is, { 1, 0, 0 }, { 0, 0, -1 }, { 0, 1, 0 }, 0.5f);
    addFace(vs, is, { -1, 0, 0 }, { 0, 0, 1 }, { 0, 1, 0 }, 0.5f);
    addFace(vs, is, { 0, 1, 0 }, { 1, 0, 0 }, { 0, 0, -1 }, 0.5f);
    addFace(vs, is, { 0, -1, 0 }, { 1, 0, 0 }, { 0, 0, 1 }, 0.5f);
    addFace(vs, is, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 1, 0 }, 0.5f);
    addFace(vs, is, { 0, 0, -1 }, { -1, 0, 0 }, { 0, 1, 0 }, 0.5f);
    return std::make_unique<Mesh>(vs, is);
}

std::unique_ptr<Mesh> Mesh::quad()
{
    std::vector<Vertex> vs;
    std::vector<unsigned int> is;
    addFace(vs, is, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 1, 0 }, 0.0f);
    return std::make_unique<Mesh>(vs, is);
}

std::unique_ptr<Mesh> Mesh::cylinder(int seg)
{
    const float pi2 = 6.28318530718f;
    std::vector<Vertex> vs;
    std::vector<unsigned int> is;

    for (int i = 0; i <= seg; ++i) {
        float a = pi2 * i / seg, c = std::cos(a), s = std::sin(a);
        vs.push_back({ { 0.5f * c, -0.5f, 0.5f * s }, { c, 0, s }, { (float)i / seg, 0 } });
        vs.push_back({ { 0.5f * c,  0.5f, 0.5f * s }, { c, 0, s }, { (float)i / seg, 1 } });
    }
    for (int i = 0; i < seg; ++i) {
        unsigned int b = 2 * i;
        for (unsigned int k : { b, b + 1, b + 2, b + 1, b + 3, b + 2 }) is.push_back(k);
    }
    for (int cap = 0; cap < 2; ++cap) {
        float y = cap ? 0.5f : -0.5f, ny = cap ? 1.0f : -1.0f;
        unsigned int centre = static_cast<unsigned int>(vs.size());
        vs.push_back({ { 0, y, 0 }, { 0, ny, 0 }, { 0.5f, 0.5f } });
        for (int i = 0; i <= seg; ++i) {
            float a = pi2 * i / seg, c = std::cos(a), s = std::sin(a);
            vs.push_back({ { 0.5f * c, y, 0.5f * s }, { 0, ny, 0 }, { 0.5f + 0.5f * c, 0.5f + 0.5f * s } });
        }
        for (int i = 0; i < seg; ++i) {
            unsigned int r = centre + 1 + i;
            if (cap) { is.push_back(centre); is.push_back(r + 1); is.push_back(r); }
            else     { is.push_back(centre); is.push_back(r);     is.push_back(r + 1); }
        }
    }
    return std::make_unique<Mesh>(vs, is);
}

std::unique_ptr<Mesh> Mesh::sphere(int stacks, int sectors)
{
    const float pi = 3.14159265359f;
    std::vector<Vertex> vs;
    std::vector<unsigned int> is;
    for (int i = 0; i <= stacks; ++i) {
        float phi = pi * i / stacks, y = std::cos(phi), r = std::sin(phi);
        for (int j = 0; j <= sectors; ++j) {
            float th = 2.0f * pi * j / sectors;
            glm::vec3 n(r * std::cos(th), y, r * std::sin(th));
            vs.push_back({ 0.5f * n, n, { (float)j / sectors, (float)i / stacks } });
        }
    }
    for (int i = 0; i < stacks; ++i)
        for (int j = 0; j < sectors; ++j) {
            unsigned int k1 = i * (sectors + 1) + j, k2 = k1 + sectors + 1;
            for (unsigned int k : { k1, k1 + 1, k2, k1 + 1, k2 + 1, k2 }) is.push_back(k);
        }
    return std::make_unique<Mesh>(vs, is);
}
