#include "RenderTarget.h"

#include "Game.h"

namespace EcoSort {

    void RenderTarget::addAttachment(TextureDescriptor descriptor) {
        auto& pair = m_attachments.emplace_back(std::make_shared<Texture>(), descriptor);
        Texture* texture = pair.first.get();
        m_attachments.back().second = descriptor;
        texture->setData(m_width, m_height, descriptor);
        if (descriptor.type == TextureType::DEPTH) {
            m_framebuffer.addDepthAttachment(*texture);
            return;
        }
        m_framebuffer.addColorAttachment(*texture, m_attachments.size() - 1);
        auto* buffers = new unsigned int[m_attachments.size()];
        for (int i = 0; i < m_attachments.size(); i++) {
            buffers[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        glDrawBuffers(m_attachments.size(), buffers);
        delete[] buffers;
    }

    void RenderTarget::resize(int width, int height) {
        m_width = width;
        m_height = height;

        for (auto& [ tex, desc ] : m_attachments) {
            tex->setData(m_width, m_height, desc);
        }
    }

    void RenderTarget::use() {
        for (int i = 0; i < m_attachments.size(); i++) {
            Texture::setUnit(i);
            m_attachments[i].first->bind();
        }
        Texture::setUnit(0);
    }

    void RenderTarget::bind() {
        glViewport(0, 0, m_width, m_height);
        m_framebuffer.bind();
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOGGER.error("Framebuffer is not complete!");
        }
    }
    
}
