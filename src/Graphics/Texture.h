#pragma once

#include "VertexBuffer.h"

namespace EcoSort {

    enum class TextureType {
        COLOUR,
        DEPTH
    };

    struct TextureDescriptor {
        TextureType type;
        DataType dataType;
        bool normalised;
    };
    
    class Texture {
    public:

        Texture();
        ~Texture();

        void bind();
        static void setUnit(int unit);

        void setData(const char* path);
        
        void setData(const char* data, int width, int height, bool normalised);
        void setData(const unsigned char* data, int width, int height, bool normalised);
        void setData(const short* data, int width, int height, bool normalised);
        void setData(const unsigned short* data, int width, int height, bool normalised);
        void setData(const int* data, int width, int height, bool normalised);
        void setData(const unsigned int* data, int width, int height, bool normalised);
        void setData(const float* data, int width, int height, bool normalised);
        void setData(const double* data, int width, int height, bool normalised);
        
        void setData(int width, int height, TextureDescriptor descriptor) { setData(nullptr, width, height, descriptor); }
        void setData(const void* data, int width, int height, TextureDescriptor descriptor);

    private:

        unsigned int m_handle;

        friend class Framebuffer;
        
    };
    
}
