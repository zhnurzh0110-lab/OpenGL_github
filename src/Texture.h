#pragma once
#include <vector>

// 2D OpenGL texture. All art in this project is generated procedurally on the CPU
// (see Texture.cpp), uploaded once, and mipmapped with glGenerateMipmap.
class Texture {
public:
    Texture() = default;
    ~Texture();
    Texture(Texture&& o) noexcept;
    Texture& operator=(Texture&& o) noexcept;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void bind(unsigned int unit = 0) const;
    unsigned int id() const { return id_; }

    static Texture fromRGBA(int w, int h, const std::vector<unsigned char>& pixels);
    static Texture renderTarget(int w, int h);   // empty RGBA8 colour attachment for a Framebuffer

    // Procedural textures
    static Texture white();
    static Texture tiles();      // polished vault floor tiles
    static Texture concrete();   // reinforced concrete panels with bolts
    static Texture metal();      // brushed steel
    static Texture money();      // banknote

private:
    unsigned int id_ = 0;
};
