#include "Shader.h"

#include <fstream>
#include <sstream>

#include "Game.h"

namespace EcoSort {

    // Declare and compile a shader from the source found in a file at path.
    Shader::Shader(const char* path, ShaderType type) : m_type(type) {

        LOGGER.debug("Reading shader source from path: {}", path);

        m_handle = glCreateShader(static_cast<GLenum>(type));
        std::string shaderSource;

        std::ifstream file(path);
        LOGGER.assert(file.is_open(), "Failed to open shader file: {}", path);

        std::stringstream buffer;
        buffer << file.rdbuf() << '\0';

        file.close();

        shaderSource = buffer.str();
        const char* ccsrc = shaderSource.c_str();

        glShaderSource(m_handle, 1, &ccsrc, nullptr);
        glCompileShader(m_handle);

        int success;
        char infoLog[512];
        glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(m_handle, 512, nullptr, infoLog);
            LOGGER.error("Vertex shader compilation failed: {}", infoLog);
        }
        
    }

    Shader::~Shader() {
        glDeleteShader(m_handle);
    }
    
}
