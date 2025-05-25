#pragma once

#include "Shader.h"

namespace RecyclingGame {

    class ShaderProgram {
    public:

        ShaderProgram();
        ~ShaderProgram();

        void use();
        void link();
        
        void attachShader(Shader& shader);
    

    private:

        unsigned int m_handle;
        
    };

}