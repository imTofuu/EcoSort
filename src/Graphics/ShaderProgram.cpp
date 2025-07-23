#include "ShaderProgram.h"

#include <Game.h>

namespace EcoSort {

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
        link();
    }

    int ShaderProgram::getUniformHandle(const char* name) {
        int location = glGetUniformLocation(m_handle, name);
        LOGGER.weakAssert(location != -1, "Uniform '{}' not found in shader program", name);
        return location;
    }

    void ShaderProgram::setByte(const char* name, char value) {
        use();
        glUniform1i(getUniformHandle(name), value);
    }

    void ShaderProgram::setUByte(const char* name, unsigned char value) {
        use();
        glUniform1i(getUniformHandle(name), value);
    }

    void ShaderProgram::setShort(const char* name, short value) {
        use();
        glUniform1i(getUniformHandle(name), value);
    }

    void ShaderProgram::setUShort(const char* name, unsigned short value) {
        use();
        glUniform1i(getUniformHandle(name), value);
    }

    void ShaderProgram::setInt(const char* name, int value) {
        use();
        glUniform1i(getUniformHandle(name), value);
    }

    void ShaderProgram::setUInt(const char* name, unsigned int value) {
        use();
        glUniform1i(getUniformHandle(name), value);
    }

    void ShaderProgram::setFloat(const char* name, float value) {
        use();
        glUniform1f(getUniformHandle(name), value);
    }

    void ShaderProgram::setDouble(const char* name, double value) {
        use();
        glUniform1d(getUniformHandle(name), value);
    }

    void ShaderProgram::setMat2(const char* name, const float* value) {
        use();
        glUniformMatrix2fv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setDMat2(const char* name, const double* value) {
        use();
        glUniformMatrix2dv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setMat2x3(const char* name, const float* value) {
        use();
        glUniformMatrix2x3fv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setDMat2x3(const char* name, const double* value) {
        use();
        glUniformMatrix2x3dv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setMat2x4(const char* name, const float* value) {
        use();
        glUniformMatrix2x4fv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setDMat2x4(const char* name, const double* value) {
        use();
        glUniformMatrix2x4dv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setMat3(const char* name, const float* value) {
        use();
        glUniformMatrix3fv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setDMat3(const char* name, const double* value) {
        use();
        glUniformMatrix3dv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setMat3x2(const char* name, const float* value) {
        use();
        glUniformMatrix3x2fv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setDMat3x2(const char* name, const double* value) {
        use();
        glUniformMatrix3x2dv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setMat3x4(const char* name, const float* value) {
        use();
        glUniformMatrix3x4fv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setDMat3x4(const char* name, const double* value) {
        use();
        glUniformMatrix3x4dv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setMat4(const char* name, const float* value) {
        use();
        glUniformMatrix4fv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setDMat4(const char* name, const double* value) {
        use();
        glUniformMatrix4dv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setMat4x2(const char* name, const float* value) {
        use();
        glUniformMatrix4x2fv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setDMat4x2(const char* name, const double* value) {
        use();
        glUniformMatrix4x2dv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setMat4x3(const char* name, const float* value) {
        use();
        glUniformMatrix4x3fv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setDMat4x3(const char* name, const double* value) {
        use();
        glUniformMatrix4x3dv(getUniformHandle(name), 1, GL_FALSE, value);
    }

    void ShaderProgram::setBytes(const char* name, const char* value, unsigned int num) {
        use();
        int location = getUniformHandle(name);
        auto ptrvalue = reinterpret_cast<const int*>(value);

        switch (num) {
            case 1:
                glUniform1iv(location, 1, ptrvalue);
                break;
            case 2:
                glUniform2iv(location, 1, ptrvalue);
                break;
            case 3:
                glUniform3iv(location, 1, ptrvalue);
                break;
            case 4:
                glUniform4iv(location, 1, ptrvalue);
                break;
            default:
                LOGGER.error("Invalid num for setBytes");
        }
    }

    void ShaderProgram::setUBytes(const char* name, const unsigned char* value, unsigned int num) {
        use();
        int location = getUniformHandle(name);
        auto ptrvalue = reinterpret_cast<const int*>(value);

        switch (num) {
            case 1:
                glUniform1iv(location, 1, ptrvalue);
                break;
            case 2:
                glUniform2iv(location, 1, ptrvalue);
                break;
            case 3:
                glUniform3iv(location, 1, ptrvalue);
                break;
            case 4:
                glUniform4iv(location, 1, ptrvalue);
                break;
            default:
                LOGGER.error("Invalid num for setUBytes");
        }
    }

    void ShaderProgram::setShorts(const char* name, const short* value, unsigned int num) {
        use();
        int location = getUniformHandle(name);
        auto ptrvalue = reinterpret_cast<const int*>(value);

        switch (num) {
            case 1:
                glUniform1iv(location, 1, ptrvalue);
                break;
            case 2:
                glUniform2iv(location, 1, ptrvalue);
                break;
            case 3:
                glUniform3iv(location, 1, ptrvalue);
                break;
            case 4:
                glUniform4iv(location, 1, ptrvalue);
                break;
            default:
                LOGGER.error("Invalid num for setShorts");
        }
    }

    void ShaderProgram::setUShorts(const char* name, const unsigned short* value, unsigned int num) {
        use();
        int location = getUniformHandle(name);
        auto ptrvalue = reinterpret_cast<const int*>(value);

        switch (num) {
            case 1:
                glUniform1iv(location, 1, ptrvalue);
                break;
            case 2:
                glUniform2iv(location, 1, ptrvalue);
                break;
            case 3:
                glUniform3iv(location, 1, ptrvalue);
                break;
            case 4:
                glUniform4iv(location, 1, ptrvalue);
                break;
            default:
                LOGGER.error("Invalid num for setUShorts");
        }
    }

    void ShaderProgram::setInts(const char* name, const int* value, unsigned int num) {
        use();
        int location = getUniformHandle(name);
        auto ptrvalue = reinterpret_cast<const int*>(value);

        switch (num) {
            case 1:
                glUniform1iv(location, 1, ptrvalue);
                break;
            case 2:
                glUniform2iv(location, 1, ptrvalue);
                break;
            case 3:
                glUniform3iv(location, 1, ptrvalue);
                break;
            case 4:
                glUniform4iv(location, 1, ptrvalue);
                break;
            default:
                LOGGER.error("Invalid num for setInts");
        }
    }

    void ShaderProgram::setUInts(const char* name, const unsigned int* value, unsigned int num) {
        use();
        int location = getUniformHandle(name);
        auto ptrvalue = reinterpret_cast<const int*>(value);

        switch (num) {
            case 1:
                glUniform1iv(location, 1, ptrvalue);
                break;
            case 2:
                glUniform2iv(location, 1, ptrvalue);
                break;
            case 3:
                glUniform3iv(location, 1, ptrvalue);
                break;
            case 4:
                glUniform4iv(location, 1, ptrvalue);
                break;
            default:
                LOGGER.error("Invalid num for setUInts");
        }
    }

    void ShaderProgram::setFloats(const char* name, const float* value, unsigned int num) {
        use();
        int location = getUniformHandle(name);
        auto ptrvalue = reinterpret_cast<const float*>(value);

        switch (num) {
            case 1:
                glUniform1fv(location, 1, ptrvalue);
                break;
            case 2:
                glUniform2fv(location, 1, ptrvalue);
                break;
            case 3:
                glUniform3fv(location, 1, ptrvalue);
                break;
            case 4:
                glUniform4fv(location, 1, ptrvalue);
                break;
            default:
                LOGGER.error("Invalid num for setFloats");
        }
    }

    void ShaderProgram::setDoubles(const char* name, const double* value, unsigned int num) {
        use();
        int location = getUniformHandle(name);
        auto ptrvalue = reinterpret_cast<const double*>(value);

        switch (num) {
            case 1:
                glUniform1dv(location, 1, ptrvalue);
                break;
            case 2:
                glUniform2dv(location, 1, ptrvalue);
                break;
            case 3:
                glUniform3dv(location, 1, ptrvalue);
                break;
            case 4:
                glUniform4dv(location, 1, ptrvalue);
                break;
            default:
                LOGGER.error("Invalid num for setDoubles");
        }
    }
    
}