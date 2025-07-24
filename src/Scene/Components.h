#pragma once

#include "glm/detail/type_quat.hpp"
#include "glm/fwd.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace EcoSort {

    struct TransformComponent {
        
        glm::vec3 position = glm::vec3(0.0f),
                    scale = glm::vec3(1.0f);

        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

        glm::mat4 getTransformation();
        
    };

    struct CameraComponent {

        float fov = 45.0f;
        
    };

    struct LightComponent {

        enum class LightType {
            POINT = 0,
            SPOT = 1,
            DIRECTIONAL = 2
        };
        
        glm::vec3 colour = glm::vec3(1.0f);
        LightType type = LightType::POINT;
        
    };
    
}
