#pragma once
#include "Texture.h"

// Off-screen render target (colour texture + depth renderbuffer) used for the CCTV monitor.
class Framebuffer {
public:
    Framebuffer(int width, int height);
    ~Framebuffer();
    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    void bind() const;
    static void unbind();
    const Texture& color() const { return color_; }
    int width() const { return w_; }
    int height() const { return h_; }

private:
    unsigned int fbo_ = 0, depthRbo_ = 0;
    int w_, h_;
    Texture color_;
};
