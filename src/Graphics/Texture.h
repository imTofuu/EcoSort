#pragma once

namespace RecyclingGame {

    class Texture {
    public:

        Texture();
        ~Texture();

        void bind(int slot);

        void setData(const char* path);
        void setData(unsigned char* data, int width, int height);

    private:

        unsigned int m_handle;
        
    };
    
}