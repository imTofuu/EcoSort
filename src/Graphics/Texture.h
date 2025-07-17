#pragma once

#include <glad/gl.h>

#include "VertexBuffer.h"

namespace EcoSort {

    enum class TextureType {
        COLOUR,
        DEPTH
    };

    class Texture {
    public:

        Texture();
        ~Texture();

        void bind(int slot);

        void setData(const char* path);
        void setData(unsigned char* data, int width, int height)
            { setData(data, width, height, DataType::UNSIGNED_BYTE); }
        void setData(unsigned char* data, int width, int height, DataType dataType);
        void setData(unsigned char* data, int width, int height, TextureType textureType);

    private:

        unsigned int m_handle;

        friend class Framebuffer;
        
    };
    
}
