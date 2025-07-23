#pragma once

#include <vector>

#include "Framebuffer.h"
#include "Texture.h"

namespace EcoSort {

    class RenderTarget {
    public:
        
        RenderTarget(int width, int height) : m_width(width), m_height(height) {}

        void addAttachment(TextureDescriptor descriptor);

        void resize(int width, int height);

        void use();
        void bind();

    private:

        Framebuffer m_framebuffer;
        std::vector<std::pair<std::shared_ptr<Texture>, TextureDescriptor>> m_attachments;

        int m_width, 
            m_height;

        friend class Renderer;
        
    };
    
}
