#include "Game.h"

#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "AssetFetcher.h"
#include <../demo/Clock.h>
#include "Interface/Window.h"
#include "Scene/Components.h"
#include "Scene/Object.h"
#include <dynamics/q3Contact.h>

#undef assert

namespace EcoSort {
    Game *Game::s_instance = nullptr;

    void glfwErrorCallback(int error, const char* description) {
        static Logger logger("GLFW");
        logger.error("Error {}: {}", error, description);
    }

    std::unordered_map<q3Body*, std::vector<q3Body*>> touchingConveyors;

    void Game::BeginContact(const q3ContactConstraint* contact) {
        // This will not work if i ever need anything else to have user data

        if (auto userData = contact->bodyA->GetUserData(); userData) {
            touchingConveyors[contact->bodyA].emplace_back(contact->bodyB);
        } else if (auto userData = contact->bodyB->GetUserData(); userData) {
            touchingConveyors[contact->bodyB].emplace_back(contact->bodyA);
        }
    }

    void Game::EndContact(const q3ContactConstraint* contact) {
        if (auto userData = contact->bodyA->GetUserData(); userData) {
            std::erase(touchingConveyors[contact->bodyA], contact->bodyB);
        } else if (auto userData = contact->bodyB->GetUserData(); userData) {
            std::erase(touchingConveyors[contact->bodyB], contact->bodyA);
        }
    }



    void Game::run() {
        m_logger.info("Initialising game");

        // If GLFW has an error, it will call this function where I log the error.
        glfwSetErrorCallback(glfwErrorCallback);

        int result;

        // Initialise GLFW so a window can be created.
        result = glfwInit();
        m_logger.strongAssert(result, "Failed to initialize GLFW");
        m_logger.debug("Initialised GLFW");

        // Specify the version of OpenGL that will be used as a window hint. The window hints will apply to all windows
        // created after they are set. This project uses GL 4.1 core, since it is the most recent version supported by
        // macOS.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a new scope so the window will be destroyed once the main loop has finished.
        {
#ifdef RG_DEBUG
            Window window("EcoSort", 750, 750);
#else
            Window window("EcoSort");
#endif
            m_logger.info("Initialised window");

            m_physicsScene.SetContactListener(this);

            // MAIN MENU -----------------------------------------------------|>

            {
                Object menuCamera = m_menuScene.createObject();
                auto menuCameraComp = menuCamera.addComponent<CameraComponent>();
                auto menuCameraTransform = menuCamera.addComponent<TransformComponent>();
                menuCameraTransform->position = glm::vec3(-1.0f, 5.0f, -10.0f);
                menuCameraTransform->rotation = glm::quatLookAt(-glm::normalize(menuCameraTransform->position), glm::vec3(0.0f, 1.0f, 0.0f));

                Object sun = m_menuScene.createObject();
                auto lightTransform = sun.addComponent<TransformComponent>();
                auto lightComp = sun.addComponent<LightComponent>();
                lightTransform->rotation = glm::quatLookAt(-glm::normalize(glm::vec3(-5.0f, 20.0f, -1.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
                lightComp->type = LightComponent::LightType::DIRECTIONAL;
            }

            // Still main menu, but these references are needed outside the above scope

            Object menuList = m_menuScene.createObject();
            auto menuListTransform = menuList.addComponent<Transform2DComponent>();
            auto menuListComp = menuList.addComponent<GUIFrameComponent>();
            menuListTransform->position = {
                { 0, 0 },
                { 0.5f, 0.5f }
            };
            menuListTransform->size = {
                { 0, 0 },
                { 0.5, 0.5 }
            };

            // This is the worst code maybe i have ever written
            auto& [ playButton, playButtonTransform ] =
                *menuListComp->guis.emplace_back(
                    std::make_shared<std::pair<GUIComponent, Transform2DComponent>>()
                    );
            playButtonTransform.position = {
                { 0, 0 },
                { 0.0f, -0.15f }
            };
            playButtonTransform.size = {
                { 0, 0 },
                { 1, 0.2 }
            };
            playButton.image = std::make_shared<Texture>();
            playButton.image->setData("res/UI/Play.png");
            
            auto& [ quitButton, quitButtonTransform ] =
                *menuListComp->guis.emplace_back(
                    std::make_shared<std::pair<GUIComponent, Transform2DComponent>>()
                    );
            quitButtonTransform.position = {
                { 0, 0 },
                { 0.0f, 0.15f }
            };
            quitButtonTransform.size = {
                { 0, 0 },
                { 1, 0.2 }
            };
            quitButton.image = std::make_shared<Texture>();
            quitButton.image->setData("res/UI/Quit.png");

            {
                Object gameCamera = m_gameScene.createObject();
                auto gameCameraComp = gameCamera.addComponent<CameraComponent>();
                auto gameCameraTransform = gameCamera.addComponent<TransformComponent>();
                gameCameraTransform->position = glm::vec3(-150.0f, 75.0f, 0.0f);
                gameCameraTransform->rotation = glm::quatLookAt(-glm::normalize(gameCameraTransform->position), glm::vec3(0.0f, 1.0f, 0.0f));

                Object gameSun = m_gameScene.createObject();
                auto gameSunTransform = gameSun.addComponent<TransformComponent>();
                auto gameSunLight = gameSun.addComponent<LightComponent>();
                gameSunTransform->rotation = glm::quatLookAt(-glm::normalize((gameCameraTransform->position - glm::vec3(10.0f))), glm::vec3(0.0f, 1.0f, 0.0f));
                gameSunLight->type = LightComponent::LightType::DIRECTIONAL;
                gameSunLight->colour = glm::vec3(0.5f);

                Object test = m_gameScene.createObject();
                auto testTransform = test.addComponent<TransformComponent>();
                auto testMesh = test.addComponent<Mesh>();
                auto testRigidBody = test.addComponent<RigidBodyComponent>();
                testTransform->position = glm::vec3(0.0f, 10.0f, -6 * 11.5f);
                testTransform->scale = glm::vec3(5.0f);
                test.setComponent(*AssetFetcher::meshFromPath("res/Models/Suzanne.obj"));
                testMesh->setPrimaryTexture("res/Textures/img.png");
                testRigidBody->bodyType = eDynamicBody;
                testRigidBody->scale = { 10.0f, 10.0f, 10.0f };

                for (int i = 0; i < 7; i++) {
                    Object conveyor = m_gameScene.createObject();
                    auto conveyorTransform = conveyor.addComponent<TransformComponent>();
                    auto conveyorMesh = conveyor.addComponent<Mesh>();
                    auto conveyorRigidBody = conveyor.addComponent<RigidBodyComponent>();
                    conveyor.addComponent<ConveyorComponent>();

                    conveyorTransform->position = glm::vec3(0.0f, 0.0f, (-6 * 11.5f) + 23 * i);

                    auto conveyorMeshPath = std::format("res/Models/{}Conveyor.obj", i == 3 || i == 5 ? "Short" : "");
                    
                    conveyor.setComponent(*AssetFetcher::meshFromPath(conveyorMeshPath.c_str()));
                    conveyorMesh->setPrimaryTexture("res/Textures/white.png");
                    conveyorRigidBody->bodyType = eStaticBody;
                    conveyorRigidBody->scale = { 25.0f, 3.0f, 23.0f };
                    conveyorRigidBody->userData = new BOO::ComponentRef(conveyorTransform);
                    
                    Object light = m_gameScene.createObject();
                    auto lightTransform = light.addComponent<TransformComponent>();
                    auto lightComp = light.addComponent<LightComponent>();
                    lightTransform->position = conveyorTransform->position + glm::vec3(0, 5.0f, 0);
                    lightComp->type = LightComponent::LightType::POINT;
                    lightComp->colour = glm::vec3(1.0f, 0.0f, 0.0f);
                    lightComp->distance = 100.0f;

                    if (i == 3 || i == 5) {
                        Object pusher = m_gameScene.createObject();
                        auto pusherTransform = pusher.addComponent<TransformComponent>();
                        auto pusherMesh = pusher.addComponent<Mesh>();
                        auto pusherRigidBody = pusher.addComponent<RigidBodyComponent>();
                        auto pusherComp = pusher.addComponent<PusherComponent>();
                        pusherTransform->position = glm::vec3(0.0f, 0.0f, (-6 * 11.5f) + 23 * i);
                        pusherTransform->rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
                        pusher.setComponent(*AssetFetcher::meshFromPath("res/Models/Pusher.obj"));
                        pusherMesh->setPrimaryTexture("res/Textures/white.png");
                        pusherRigidBody->bodyType = eStaticBody;
                        pusherRigidBody->scale = { 0.15, 100.0f, 23.0f };
                        pusherRigidBody->offset = { -12.5f, 0, 0.0f };
                        pusherComp->activationKey = i == 3 ? Key::Q : Key::E;
                    }
                }
            }

            double startTime = glfwGetTime();
            int frames = 0;
            
            Clock physicsClock;

            // While the window is open, i.e. the operating system has not requested for it to be closed.
            while (window.isOpen()) {

                double dt = glfwGetTime() - startTime;
                startTime = glfwGetTime();

                // Poll events in GLFW, which will handle OS events and user interfaces, such as the keyboard and mouse.
                glfwPollEvents();

                // Do gui stuff

                Interface& interface = window.getInterface();

                auto mouseX = interface.getMouseX();
                auto mouseY = interface.getMouseY();
                auto mouseEnabled = interface.getMouseButtonEnabledState(MouseButton::LEFT);

                for (auto& [ guiFrame, frameTransform ] : m_activeScene.findAll<GUIFrameComponent, Transform2DComponent>()) {

                    TransformComponent absoluteFrameTransform = window.getRenderer()->getAbsoluteTransform2D(*frameTransform);

                    for (auto& guiptr : guiFrame->guis) {

                        auto& [ gui, transform ] = *guiptr;

                        TransformComponent absoluteTransform = Renderer::getRelativeTransform2D(transform, absoluteFrameTransform);

                        gui.isHovered = mouseX >= absoluteTransform.position.x - (absoluteTransform.scale.x / 2) &&
                            mouseX <= absoluteTransform.position.x + (absoluteTransform.scale.x / 2) &&
                            mouseY >= absoluteTransform.position.y - (absoluteTransform.scale.y / 2) &&
                            mouseY <= absoluteTransform.position.y + (absoluteTransform.scale.y / 2);

                        gui.isClicked = mouseEnabled && gui.isHovered;
                        
                    }
                }

                // physics

                auto physicsQuery = m_activeScene.findAll<RigidBodyComponent, TransformComponent>();
                for (auto& [ rigidBody, transform ] : physicsQuery) {
                    
                    auto rotAxis = glm::axis(transform->rotation);
                    q3Vec3 q3RotationAxis = { rotAxis.x, rotAxis.y, rotAxis.z };
                    if (!rigidBody->body) {
                        q3BodyDef bodyDef;

                        bodyDef.userData = rigidBody->userData;
                        
                        bodyDef.position = { transform->position.x, transform->position.y, transform->position.z };
                        bodyDef.axis = q3RotationAxis;
                        bodyDef.angle = glm::angle(transform->rotation);
                        bodyDef.bodyType = rigidBody->bodyType;
                        
                        rigidBody->body = m_physicsScene.CreateBody(bodyDef);

                        q3BoxDef boxDef;
                        q3Transform boxTransform;
                        q3Identity(boxTransform);
                        boxTransform.position = rigidBody->offset;
                        boxTransform.rotation = q3Quaternion(rigidBody->rotationOffsetAxis, rigidBody->rotationOffsetAngle).ToMat3();

                        boxDef.SetRestitution(0);

                        boxDef.Set(boxTransform, rigidBody->scale);
                        rigidBody->body->AddBox(boxDef);
                    }
                    rigidBody->body->SetTransform({
                        transform->position.x,
                        transform->position.y,
                        transform->position.z
                    }, q3RotationAxis, glm::angle(transform->rotation));
                }

                float time = physicsClock.Start();

                m_physicsScene.SetIterations(8);

                static float accumulator = 0.0f;
                accumulator += time;

                while (accumulator >= 1.0f / 60) {
                    m_physicsScene.Step();
                    accumulator -= 1.0f / 60;
                }

                physicsClock.Stop();

                for (auto& [ rigidBody, transform ] : physicsQuery) {

                    auto& newTransform = rigidBody->body->GetTransform();
                    glm::mat3 rotation = {
                        { newTransform.rotation.Column0()[0], newTransform.rotation.Column1()[0], newTransform.rotation.Column2()[0] },
                        { newTransform.rotation.Column0()[1], newTransform.rotation.Column1()[1], newTransform.rotation.Column2()[1] },
                        { newTransform.rotation.Column0()[2], newTransform.rotation.Column1()[2], newTransform.rotation.Column2()[2] }
                    };

                    transform->position = { newTransform.position.x, newTransform.position.y, newTransform.position.z };
                    transform->rotation = glm::quat_cast(rotation);
                    
                }

                // game updating

                for (auto& [ conveyorBody, otherBodies ] : touchingConveyors) {
                    auto conveyorTransform = *static_cast<BOO::ComponentRef<TransformComponent>*>(conveyorBody->GetUserData());
                    
                    auto rotationMatrix = glm::mat3_cast(conveyorTransform->rotation);
                    auto conveyorDirection = rotationMatrix * glm::vec3(0.0f, 0.0f, 1.0f);

                    for (auto& otherBody : otherBodies) {
                        otherBody->SetLinearVelocity({ conveyorDirection.x * 10, 0, conveyorDirection.z * 10 });
                    }
                }

                for (auto& [ pusher, pusherTransform, pusherRigidBody ] : m_gameScene.findAll<PusherComponent, TransformComponent, RigidBodyComponent>()) {
                    if (!pusher->progress && !interface.getKeyEnabledState(pusher->activationKey)) continue;
                    pusher->progress += dt;
                    pusher->progress = glm::clamp(pusher->progress, 0.0f, 2.0f);
                    auto newOffset = 26.5f * (glm::abs(pusher->progress - 1.0f) - 1.0f);
                    pusherTransform->position.x = newOffset;
                    if (pusher->progress >= 2.0f) pusher->progress = 0.0f;
                }
                
                if (playButton.isClicked) {
                    m_physicsScene.RemoveAllBodies();

                    for (auto& [ rigidBody, transform ] : physicsQuery) {
                        rigidBody->body = nullptr;
                    }
                    
                    m_activeScene = m_gameScene;
                }
                if (quitButton.isClicked) break;

                // Swap the buffers of the window.
                window.update();

                frames++;

                // Reset and display the fps counter if a second has passed
                static double frameAccumulator = 0.0f;
                frameAccumulator += dt;
                if (frameAccumulator >= 1.0) {
                    window.setTitle(std::format("EcoSort ({} FPS)", frames).c_str());
                    frames = 0;
                    frameAccumulator = 0;
                }
            }
        }

        // Clean up GLFW
        glfwTerminate();
    }
}
