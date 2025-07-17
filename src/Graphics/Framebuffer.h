#pragma once
#include "Texture.h"

namespace EcoSort {

    class Framebuffer {
    public:
        
        Framebuffer();
        ~Framebuffer();

        void bind();
        static void unbind();

        void addColorAttachment(const Texture& texture, int index);
        void addDepthAttachment(const Texture& texture);

    private:

        unsigned int m_handle;
        
    };
    
}
