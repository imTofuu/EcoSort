#include "Texture.h"

#include "Game.h"
#include "stb_image.h"
#include "glad/gl.h"

namespace EcoSort {

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
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path, &w, &h, nullptr, 4);
        if (!data) {
            LOGGER.warn("Failed to load texture from path: {}\n"
                "Failure reason: {}", path, stbi_failure_reason());
            return;
        }
        setData(data, w, h);
        stbi_image_free(data);
    }

    void Texture::setData(unsigned char* data, int width, int height, DataType dataType) {
        GLint internalFormat = GL_RGBA;
        if (dataType == DataType::FLOAT) {
            internalFormat = GL_RGBA16F;
        }
        glTexImage2D(GL_TEXTURE_2D,
                0,
                internalFormat,
                width,
                height,
                0,
                GL_RGBA,
                static_cast<GLenum>(dataType),
                data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glGenerateMipmap(GL_TEXTURE_2D);
        
    }

    void Texture::setData(unsigned char* data, int width, int height, TextureType textureType) {
        GLint format = 0;
        GLint internalFormat = 0;

        switch (textureType) {
            case TextureType::COLOUR:
                internalFormat = GL_RGBA8;
                format = GL_RGBA;
                break;
            case TextureType::DEPTH:
                internalFormat = GL_DEPTH_COMPONENT24;
                format = GL_DEPTH_COMPONENT;
                break;
            default:
                LOGGER.warn("Unknown texture type: {}", static_cast<int>(textureType));
                internalFormat = GL_RGBA;
                format = GL_RGBA;
                break;
        }
        
        glTexImage2D(GL_TEXTURE_2D,
                0,
                internalFormat,
                width,
                height,
                0,
                format,
                GL_UNSIGNED_BYTE,
                data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glGenerateMipmap(GL_TEXTURE_2D);
        
    }
    
}
