#include "Texture.h"

#include "Game.h"
#include "stb_image.h"
#include "glad/gl.h"

namespace EcoSort {

    Texture::Texture() : m_handle(0) {
        glGenTextures(1, &m_handle);

        bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_handle);
    }

    void Texture::bind() {
        glBindTexture(GL_TEXTURE_2D, m_handle);
    }

    void Texture::setUnit(int unit) {
        glActiveTexture(GL_TEXTURE0 + unit);
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
        setData(data, w, h, true);
        stbi_image_free(data);
    }
    
    void Texture::setData(const char* data, int width, int height, bool normalised) {
        setData(data, width, height, {
            TextureType::COLOUR, DataType::BYTE, normalised
        });
    }

    void Texture::setData(const unsigned char* data, int width, int height, bool normalised) {
        setData(data, width, height, {
        TextureType::COLOUR, DataType::UNSIGNED_BYTE, normalised
        });
    }

    void Texture::setData(const short* data, int width, int height, bool normalised) {
        setData(data, width, height, {
            TextureType::COLOUR, DataType::SHORT, normalised
        });
    }

    void Texture::setData(const unsigned short* data, int width, int height, bool normalised) {
        setData(data, width, height, {
            TextureType::COLOUR, DataType::UNSIGNED_SHORT, normalised
        });
    }

    void Texture::setData(const int* data, int width, int height, bool normalised) {
        setData(data, width, height, {
            TextureType::COLOUR, DataType::INT, normalised
        });
    }

    void Texture::setData(const unsigned int* data, int width, int height, bool normalised) {
        setData(data, width, height, {
            TextureType::COLOUR, DataType::UNSIGNED_INT, normalised
        });
    }

    void Texture::setData(const float* data, int width, int height, bool normalised) {
        setData(data, width, height, {
            TextureType::COLOUR, DataType::FLOAT, normalised
        });
    }

    void Texture::setData(const double* data, int width, int height, bool normalised) {
        setData(data, width, height, {
            TextureType::COLOUR, DataType::DOUBLE, normalised
        });
    }

    void Texture::setData(const void* data, int width, int height, TextureDescriptor descriptor) {
        
        int internalFormat;
        int format = GL_RGBA;

        switch (descriptor.dataType) {
            case DataType::UNSIGNED_BYTE:
                if (descriptor.type == TextureType::DEPTH) {
                    LOGGER.warn("8 bit depth textures are not supported, using 16 bit.");
                    internalFormat = GL_DEPTH_COMPONENT16;
                    format = GL_DEPTH_COMPONENT;
                    break;
                }
                // GL_RGBA8UI if normalised is false, GL_RGBA8 if true. The same logic applies for below.
                internalFormat = GL_RGBA8UI - (0xD24 * descriptor.normalised);
                break;
            case DataType::BYTE:
                if (descriptor.type == TextureType::DEPTH) {
                    LOGGER.warn("8 bit depth textures are not supported, using 16 bit.");
                    LOGGER.warn("Unsigned byte depth textures are not supported, using signed instead.");
                    internalFormat = GL_DEPTH_COMPONENT16;
                    format = GL_DEPTH_COMPONENT;
                    break;
                }
                internalFormat = GL_RGBA8I + (0x209 * descriptor.normalised);
                break;
            case DataType::UNSIGNED_SHORT:
                if (descriptor.type == TextureType::DEPTH) {
                    internalFormat = GL_DEPTH_COMPONENT16;
                    format = GL_DEPTH_COMPONENT;
                    break;
                }
                internalFormat = GL_RGBA16UI - (0xD1B * descriptor.normalised);
                break;
            case DataType::SHORT:
                if (descriptor.type == TextureType::DEPTH) {
                    LOGGER.warn("Unsigned byte depth textures are not supported, using signed instead.");
                    internalFormat = GL_DEPTH_COMPONENT16;
                    format = GL_DEPTH_COMPONENT;
                    break;
                }
                internalFormat = GL_RGBA16I + (0x213 * descriptor.normalised);
                break;
            case DataType::UNSIGNED_INT:
                if (descriptor.type == TextureType::DEPTH) {
                    internalFormat = GL_DEPTH_COMPONENT32;
                    format = GL_DEPTH_COMPONENT;
                    break;
                }
                internalFormat = GL_RGBA32UI - (0x7468 * descriptor.normalised);
                break;
            case DataType::INT:
                if (descriptor.type == TextureType::DEPTH) {
                    LOGGER.warn("Unsigned byte depth textures are not supported, using signed instead.");
                    internalFormat = GL_DEPTH_COMPONENT32;
                    format = GL_DEPTH_COMPONENT;
                    break;
                }
                internalFormat = GL_RGBA32I + (0x211 * descriptor.normalised);
                break;
            case DataType::FLOAT:
                if (descriptor.type == TextureType::DEPTH) {
                    internalFormat = GL_DEPTH_COMPONENT32F;
                    format = GL_DEPTH_COMPONENT;
                    break;
                }
                internalFormat = GL_RGBA32F;
                break;
            case DataType::DOUBLE:
                LOGGER.warn("Double precision textures are not supported, using float instead.");
                if (descriptor.type == TextureType::DEPTH) {
                    internalFormat = GL_DEPTH_COMPONENT32F;
                    format = GL_DEPTH_COMPONENT;
                    break;
                }
                internalFormat = GL_RGBA32F;
            default:
                LOGGER.warn("Unknown data type: {}, using unsigned normalised type instead",
                    static_cast<int>(descriptor.dataType));
                internalFormat = GL_RGBA;
                break;
        }

        bind();

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            internalFormat,
            width,
            height,
            0,
            format,
            static_cast<GLenum>(descriptor.dataType),
            data);

        glGenerateMipmap(GL_TEXTURE_2D);
        
    }

    /*void Texture::setData(unsigned char* data, int width, int height, TextureDescriptor descriptor) {
        GLint internalFormat = GL_RGBA;
        if (dataType == DataType::FLOAT) {
            internalFormat = GL_RGBA16F;
        }

        int internalFormat;
        int format;

        switch (descriptor.dataType) {
            case DataType::UNSIGNED_BYTE:
                internalFormat = GL_RGBA8UI;
                format = GL_RGBA;
            case DataType::BYTE:
                internalFormat = GL_RGBA8;
                format = GL_RGBA;
                break;
            case DataType::UNSIGNED_SHORT:
            case DataType::SHORT:
                internalFormat = GL_RGBA16;
                format = GL_RGBA;
            case DataType::UNSIGNED_INT:
                case DataType::INT:
                internalFormat = GL_RGBA32UI;
            case DataType::FLOAT:
                internalFormat = GL_RGBA32F;
                format = GL_RGBA;
                break;
        }
        
        bind();
        glTexImage2D(GL_TEXTURE_2D,
                0,
                internalFormat,
                width,
                height,
                0,
                GL_RGBA,
                static_cast<GLenum>(dataType),
                data);
        
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
        bind();
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
        
    }*/
    
}
