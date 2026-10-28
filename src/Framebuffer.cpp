#include "Framebuffer.h"
#include <glad/glad.h>
#include <stdexcept>

Framebuffer::Framebuffer(int width, int height) : w_(width), h_(height)
{
    color_ = Texture::renderTarget(width, height);

    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_.id(), 0);

    glGenRenderbuffers(1, &depthRbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo_);

    bool ok = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (!ok) throw std::runtime_error("CCTV framebuffer is incomplete");
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &fbo_);
    glDeleteRenderbuffers(1, &depthRbo_);
}

void Framebuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, fbo_); }
void Framebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
