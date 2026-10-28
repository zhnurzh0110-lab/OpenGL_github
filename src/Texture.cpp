#include "Texture.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>
#include <functional>

namespace {

// ---- seamless value noise (period = number of lattice cells across the texture) ----
float hash2(int x, int y)
{
    unsigned h = (unsigned)x * 374761393u + (unsigned)y * 668265263u;
    h = (h ^ (h >> 13)) * 1274126177u;
    h ^= h >> 16;
    return (h & 0xFFFFFFu) / 16777215.0f;
}

float vnoise(float x, float y, int px, int py)
{
    int x0 = (int)std::floor(x), y0 = (int)std::floor(y);
    float fx = x - x0, fy = y - y0;
    auto wrap = [](int v, int p) { return ((v % p) + p) % p; };
    float a = hash2(wrap(x0, px),     wrap(y0, py));
    float b = hash2(wrap(x0 + 1, px), wrap(y0, py));
    float c = hash2(wrap(x0, px),     wrap(y0 + 1, py));
    float d = hash2(wrap(x0 + 1, px), wrap(y0 + 1, py));
    float u = fx * fx * (3 - 2 * fx), v = fy * fy * (3 - 2 * fy);
    return a + (b - a) * u + (c - a) * v + (a - b - c + d) * u * v;
}

float fbm(float u, float v, int cells)
{
    float sum = 0.0f, amp = 0.5f;
    for (int o = 0; o < 4; ++o) {
        int c = cells << o;
        sum += amp * vnoise(u * c, v * c, c, c);
        amp *= 0.5f;
    }
    return sum / 0.9375f;   // normalise to ~0..1
}

Texture generate(int w, int h, const std::function<glm::vec3(float, float)>& fn)
{
    std::vector<unsigned char> px((size_t)w * h * 4);
    for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x) {
            glm::vec3 c = glm::clamp(fn((x + 0.5f) / w, (y + 0.5f) / h), 0.0f, 1.0f);
            size_t i = ((size_t)y * w + x) * 4;
            px[i + 0] = (unsigned char)(c.r * 255.0f);
            px[i + 1] = (unsigned char)(c.g * 255.0f);
            px[i + 2] = (unsigned char)(c.b * 255.0f);
            px[i + 3] = 255;
        }
    return Texture::fromRGBA(w, h, px);
}

} // namespace

Texture::~Texture() { if (id_) glDeleteTextures(1, &id_); }

Texture::Texture(Texture&& o) noexcept : id_(o.id_) { o.id_ = 0; }

Texture& Texture::operator=(Texture&& o) noexcept
{
    if (this != &o) {
        if (id_) glDeleteTextures(1, &id_);
        id_ = o.id_;
        o.id_ = 0;
    }
    return *this;
}

void Texture::bind(unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id_);
}

Texture Texture::fromRGBA(int w, int h, const std::vector<unsigned char>& pixels)
{
    Texture t;
    glGenTextures(1, &t.id_);
    glBindTexture(GL_TEXTURE_2D, t.id_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);                          // mipmapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return t;
}

Texture Texture::renderTarget(int w, int h)
{
    Texture t;
    glGenTextures(1, &t.id_);
    glBindTexture(GL_TEXTURE_2D, t.id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return t;
}

Texture Texture::white()
{
    return generate(2, 2, [](float, float) { return glm::vec3(1.0f); });
}

Texture Texture::tiles()
{
    return generate(256, 256, [](float u, float v) {
        int tx = (int)(u * 2), ty = (int)(v * 2);
        float cu = u * 2 - tx, cv = v * 2 - ty;
        float edge = std::min(std::min(cu, 1 - cu), std::min(cv, 1 - cv));
        glm::vec3 base = ((tx + ty) & 1) ? glm::vec3(0.19f, 0.21f, 0.26f) : glm::vec3(0.14f, 0.16f, 0.21f);
        base += (fbm(u, v, 16) - 0.5f) * 0.08f;
        return edge < 0.018f ? glm::vec3(0.04f) : base;
    });
}

Texture Texture::concrete()
{
    return generate(256, 256, [](float u, float v) {
        float n = 0.50f + (fbm(u, v, 8) - 0.5f) * 0.30f;
        glm::vec3 c = glm::vec3(0.80f, 0.86f, 1.0f) * n;
        float pu = u * 2 - (int)(u * 2), pv = v * 2 - (int)(v * 2);       // 2 x 2 panels
        float edge = std::min(std::min(pu, 1 - pu), std::min(pv, 1 - pv));
        if (edge < 0.014f) c *= 0.45f;                                      // panel seams
        glm::vec2 bolt(std::min(pu, 1 - pu) - 0.07f, std::min(pv, 1 - pv) - 0.07f);
        if (glm::length(bolt) < 0.022f) c = glm::vec3(0.16f);               // bolts
        return c;
    });
}

Texture Texture::metal()
{
    return generate(256, 256, [](float u, float v) {
        float streak = vnoise(u * 2, v * 160, 2, 160);
        float fine   = fbm(u, v, 32);
        float s = 0.52f + (streak - 0.5f) * 0.32f + (fine - 0.5f) * 0.10f;
        return glm::vec3(s * 0.95f, s, s * 1.06f);
    });
}

Texture Texture::money()
{
    return generate(128, 64, [](float u, float v) {
        glm::vec3 c(0.30f, 0.54f, 0.32f);
        c += 0.04f * std::sin(u * 260.0f) * std::sin(v * 90.0f);              // guilloche-ish pattern
        c += (fbm(u, v, 8) - 0.5f) * 0.10f;
        float dx = (u - 0.5f) * 2.0f, dy = (v - 0.5f) * 1.0f;
        float d = std::sqrt(dx * dx + dy * dy * 4.0f) * 0.5f;
        if (d < 0.20f) c = glm::vec3(0.78f, 0.88f, 0.70f);                    // portrait medallion
        if (d > 0.20f && d < 0.24f) c = glm::vec3(0.12f, 0.30f, 0.16f);
        if (std::min(u, 1 - u) < 0.045f || std::min(v, 1 - v) < 0.09f) c = glm::vec3(0.80f, 0.92f, 0.76f);
        return c;
    });
}
