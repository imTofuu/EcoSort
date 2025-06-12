#pragma once

#include <glad/gl.h>

namespace EcoSort {

    enum class ShaderType {
        VERT = GL_VERTEX_SHADER,
        FRAG = GL_FRAGMENT_SHADER
    };

    class Shader {
    public:

        Shader(const char* path, ShaderType type);
        ~Shader();

        ShaderType getType() { return m_type; }

    private:

        unsigned int m_handle;
        ShaderType m_type;

        friend class ShaderProgram;
        
    };
    
}