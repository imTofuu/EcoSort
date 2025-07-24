#pragma once

#include "Texture.h"

namespace EcoSort {

    class Framebuffer {
    public:
        
        Framebuffer();
        ~Framebuffer();

        void bind();
        static void unbind();

        void addColorAttachment(Texture& texture, int index);
        void addDepthAttachment(Texture& texture);

    private:

        unsigned int m_handle;

        friend class Renderer;
        
    };
    
}
