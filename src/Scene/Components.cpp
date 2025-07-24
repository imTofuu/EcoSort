#include "Components.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

namespace EcoSort {

    glm::mat4 TransformComponent::getTransformation() {
        auto translation = glm::translate(glm::mat4(1.0f), position);

        auto rotation = glm::mat4_cast(this->rotation);
        
        auto scale = glm::scale(glm::mat4(1.0f), this->scale);
        return translation * rotation * scale;
    }
    
}
