#include "Components.h"

#include "glm/gtc/matrix_transform.hpp"

namespace EcoSort {

    glm::mat4 TransformComponent::getTransformation() {
        auto translation = glm::translate(glm::mat4(1.0f), position);

        auto rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0));
        auto rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0));
        auto rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1));
        auto rotation = rotZ * rotY * rotX;
        
        auto scale = glm::scale(glm::mat4(1.0f), this->scale);
        return scale * rotation * translation;
    }
    
}
