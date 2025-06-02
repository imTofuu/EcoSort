#include "Texture.h"

#include "Game.h"
#include "stb_image.h"
#include "glad/gl.h"

namespace RecyclingGame {

    Texture::Texture() : m_handle(0) {
        glGenTextures(1, &m_handle);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_handle);
    }

    void Texture::bind(int slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_handle);
    }

    void Texture::setData(const char* path) {
        int w, h;
        unsigned char* data = stbi_load(path, &w, &h, nullptr, 4);
        if (!data) {
            LOGGER.warn("Failed to load texture from path: {}\n"
                "Failure reason: {}", path, stbi_failure_reason());
            return;
        }
        setData(data, w, h);
        stbi_image_free(data);
    }

    void Texture::setData(unsigned char* data, int width, int height) {
        
        glTexImage2D(GL_TEXTURE_2D,
                0,
                GL_RGBA,
                width,
                height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
    }

    
}
