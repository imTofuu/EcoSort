#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace EcoSort {

    struct TransformComponent {
        
        glm::vec3 position = glm::vec3(0.0f), 
                    rotation = glm::vec3(0.0f),
                    scale = glm::vec3(1.0f);

        glm::mat4 getTransformation();
        
    };

    struct CameraComponent {

        float fov = 45.0f;
        
    };
    
}
