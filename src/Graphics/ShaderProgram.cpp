#include "ShaderProgram.h"

#include <Game.h>

namespace RecyclingGame {

    ShaderProgram::ShaderProgram() {
        m_handle = glCreateProgram();
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(m_handle);
    }

    void ShaderProgram::use() {
        glUseProgram(m_handle);
    }

    // Link the program based on the shaders that are currently attached and check for success.
    void ShaderProgram::link() {
        glLinkProgram(m_handle);

        int success;
        char infoLog[512];
        glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_handle, 512, nullptr, infoLog);
            LOGGER.error("Shader program linking failed: {}", infoLog);
        }
    }

    void ShaderProgram::attachShader(Shader& shader) {
        glAttachShader(m_handle, shader.m_handle);
    }


    
}