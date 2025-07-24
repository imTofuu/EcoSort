#include "Framebuffer.h"

#include <glad/gl.h>

namespace EcoSort {

    Framebuffer::Framebuffer() : m_handle(0){
        glGenFramebuffers(1, &m_handle);
    }

    Framebuffer::~Framebuffer() {
        glDeleteFramebuffers(1, &m_handle);
    }

    void Framebuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    }

    void Framebuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::addColorAttachment(Texture& texture, int index) {
        bind();
        texture.bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0 + index,
            GL_TEXTURE_2D,
            texture.m_handle,
            0
            );
    }

    void Framebuffer::addDepthAttachment(Texture& texture) {
        bind();
        texture.bind();
        glFramebufferTexture2D(GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            texture.m_handle,
            0
        );
    }
    
}
