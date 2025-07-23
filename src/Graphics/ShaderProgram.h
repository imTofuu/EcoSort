#pragma once

#include "Shader.h"

namespace EcoSort {

    class ShaderProgram {
    public:

        ShaderProgram();
        ~ShaderProgram();

        void use();
        void link();
        
        void attachShader(Shader& shader);

        unsigned int getHandle() { return m_handle; }

        void setByte(const char* name, char value);
        void setUByte(const char* name, unsigned char value);
        void setShort(const char* name, short value);
        void setUShort(const char* name, unsigned short value);
        void setInt(const char* name, int value);
        void setUInt(const char* name, unsigned int value);
        void setFloat(const char* name, float value);
        void setDouble(const char* name, double value);

        void setBytes(const char* name, const char* value, unsigned int num);
        void setUBytes(const char* name, const unsigned char* value, unsigned int num);
        void setShorts(const char* name, const short* value, unsigned int num);
        void setUShorts(const char* name, const unsigned short* value, unsigned int num);
        void setInts(const char* name, const int* value, unsigned int num);
        void setUInts(const char* name, const unsigned int* value, unsigned int num);
        void setFloats(const char* name, const float* value, unsigned int num);
        void setDoubles(const char* name, const double* value, unsigned int num);
        
        void setMat2(const char* name, const float* value);
        void setDMat2(const char* name, const double* value);
        void setMat2x3(const char* name, const float* value);
        void setDMat2x3(const char* name, const double* value);
        void setMat2x4(const char* name, const float* value);
        void setDMat2x4(const char* name, const double* value);
        void setMat3(const char* name, const float* value);
        void setDMat3(const char* name, const double* value);
        void setMat3x2(const char* name, const float* value);
        void setDMat3x2(const char* name, const double* value);
        void setMat3x4(const char* name, const float* value);
        void setDMat3x4(const char* name, const double* value);
        void setMat4(const char* name, const float* value);
        void setDMat4(const char* name, const double* value);
        void setMat4x2(const char* name, const float* value);
        void setDMat4x2(const char* name, const double* value);
        void setMat4x3(const char* name, const float* value);
        void setDMat4x3(const char* name, const double* value);

    private:

        int getUniformHandle(const char* name);

        unsigned int m_handle;
        
    };

}