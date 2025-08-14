#pragma once

#include "Graphics/Texture.h"

#include "glm/detail/type_quat.hpp"
#include "glm/fwd.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "BOO/BOO.h"
#include "dynamics/q3Body.h"
#include "Interface/Interface.h"

namespace EcoSort {

    struct TransformComponent {
        
        glm::vec3 position = glm::vec3(0.0f),
                    scale = glm::vec3(1.0f);

        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

        glm::mat4 getTransformation();
        
    };

    struct Transform2DComponent {

        struct {
            glm::vec2 offset = glm::vec2(100.0f);
            glm::vec2 scale = glm::vec2(0.0f);
        } position;
        struct {
            glm::vec2 offset = glm::vec2(100.0f);
            glm::vec2 scale = glm::vec2(0.0f);
        } size;
        float zIndex = 0.0f;
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
        float distance = 5.0f;
        
    };

    struct GUIComponent {

        glm::vec4 colour = glm::vec4(1.0f);
        std::shared_ptr<Texture> image = nullptr;

        bool isClicked = false;
        bool isHovered = false;

    };

    struct GUIFrameComponent {

        std::vector<std::shared_ptr<std::pair<GUIComponent, Transform2DComponent>>> guis;
        
    };

    struct TagComponent {
        std::string name;
    };

    struct RigidBodyComponent {
    public:

        q3BodyType bodyType = eDynamicBody;

        q3Vec3 offset;
        q3Vec3 scale = { 1.0f, 1.0f, 1.0f };
        q3Vec3 rotationOffsetAxis = { 0.0f, 0.0f, 0.0f };
        float rotationOffsetAngle = 0.0f;
        q3Vec3 initialVelocity;
        q3Vec3 initialAngularVelocity;

    private:

        friend class Game;

        // Body is technically not safe to access until the next frame, cannot be bothered to put in more effort
        // to make this actually good
        q3Body* body = nullptr;
        
    };

    struct ConveyorComponent {
        std::vector<BOO::ComponentRef<RigidBodyComponent>> touchingRubbish; 
    };

    struct PusherComponent {
        float progress = 0.0f;
        Key activationKey = Key::Q;
    };

    struct RubbishComponent {
        enum class RubbishType {
            RUBBISH = 0,
            RECYCLING = 1,
            FOOD = 2
        } type;
    };

    struct IsGameFlagComponent {};

    struct CollectorComponent {
        RubbishComponent::RubbishType rubbishType;
    };
}
