#include "Game.h"

#include "glm/gtc/type_ptr.hpp"
#include <GLFW/glfw3.h>

#include "AssetFetcher.h"
#include "../lib/qu3e/demo/Clock.h"
#include "Interface/Window.h"
#include "Scene/Components.h"
#include "Scene/Object.h"

#undef assert

namespace EcoSort {
    Game *Game::s_instance = nullptr;

    void glfwErrorCallback(int error, const char* description) {
        static Logger logger("GLFW");
        logger.error("Error {}: {}", error, description);
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

            // MAIN MENU -----------------------------------------------------|>

            {
                Object menuCamera = m_menuScene.createObject();
                auto menuCameraComp = menuCamera.addComponent<CameraComponent>();
                auto menuCameraTransform = menuCamera.addComponent<TransformComponent>();
                menuCameraTransform->position = glm::vec3(-1.0f, 5.0f, -10.0f);
                menuCameraTransform->rotation = glm::quatLookAt(-glm::normalize(menuCameraTransform->position), glm::vec3(0.0f, 1.0f, 0.0f));

                Object object = m_menuScene.createObject();
                auto objectTransform = object.addComponent<TransformComponent>();
                auto mesh = object.addComponent<Mesh>();
                auto rigidBodyComp = object.addComponent<RigidBodyComponent>();
                objectTransform->position = glm::vec3(0.0f, 2.0f, 0.0f);
                objectTransform->rotation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0f, 0.0f, 0.0f));
                objectTransform->scale = glm::vec3(0.02f);
                rigidBodyComp->bodyType = eDynamicBody;
                rigidBodyComp->scale = { 3.6666f, 2.6666f, 1.5f };
                object.setComponent(*AssetFetcher::meshFromPath("res/Models/StanfordDragon.obj"));
                mesh->setPrimaryTexture("res/Textures/white.png");

                Object floor = m_menuScene.createObject();
                auto floorTransform = floor.addComponent<TransformComponent>();
                floorTransform->position = glm::vec3(0.0f, -1.0f, 0.0f);
                floorTransform->rotation = glm::angleAxis(-glm::pi<float>() / 8, glm::vec3(0.0f, 0, 1.0f));
                floorTransform->scale = glm::vec3(5.0f, 1.0f, 5.0f);

                auto floorRigidBody = floor.addComponent<RigidBodyComponent>();
                floorRigidBody->bodyType = eStaticBody;
                floorRigidBody->scale = { 10.0f, 2.0f, 10.0f };

                auto floorMesh = floor.addComponent<Mesh>();
                floor.setComponent(*AssetFetcher::meshFromPath("res/Models/Cube.obj"));
                floorMesh->setPrimaryTexture("res/Textures/white.png");

                Object sun = m_menuScene.createObject();
                auto lightTransform = sun.addComponent<TransformComponent>();
                auto lightComp = sun.addComponent<LightComponent>();
                lightTransform->rotation = glm::quatLookAt(-glm::normalize(glm::vec3(-5.0f, 20.0f, -1.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
                lightComp->type = LightComponent::LightType::DIRECTIONAL;
            }

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

            Object gameCamera = m_gameScene.createObject();
            auto gameCameraComp = gameCamera.addComponent<CameraComponent>();
            auto gameCameraTransform = gameCamera.addComponent<TransformComponent>();
            gameCameraTransform->position = glm::vec3(-75.0f, 30.0f, -75.0f);
            gameCameraTransform->rotation = glm::quatLookAt(-glm::normalize(gameCameraTransform->position), glm::vec3(0.0f, 1.0f, 0.0f));

            Object gameSun = m_gameScene.createObject();
            auto gameSunTransform = gameSun.addComponent<TransformComponent>();
            auto gameSunLight = gameSun.addComponent<LightComponent>();
            gameSunTransform->rotation = glm::quatLookAt(-glm::normalize(glm::vec3(-5.0f, 20.0f, -1.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
            gameSunLight->type = LightComponent::LightType::DIRECTIONAL;

            Object gameDragon = m_gameScene.createObject();
            auto gameDragonTransform = gameDragon.addComponent<TransformComponent>();
            auto gameDragonMesh = gameDragon.addComponent<Mesh>();
            auto gameDragonRigidBody = gameDragon.addComponent<RigidBodyComponent>();
            gameDragonTransform->position = glm::vec3(0.0f, 70.0f, 0.0f);
            //gameDragonTransform->rotation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(1.0f, 0.0f, 0.0f));
            gameDragonTransform->scale = glm::vec3(0.1f, 0.1f, 0.1f);
            gameDragonRigidBody->bodyType = eDynamicBody;
            gameDragonRigidBody->scale = { 18.3f, 13.3f, 7.5f };
            gameDragon.setComponent(*AssetFetcher::meshFromPath("res/Models/StanfordDragon.obj"));
            gameDragonMesh->setPrimaryTexture("res/Textures/white.png");

            Object gameFloor = m_gameScene.createObject();
            auto gameFloorTransform = gameFloor.addComponent<TransformComponent>();
            auto gameFloorRigidBody = gameFloor.addComponent<RigidBodyComponent>();
            auto gameFloorMesh = gameFloor.addComponent<Mesh>();
            gameFloorTransform->position = glm::vec3(0.0f, -1.0f, 0.0f);
            //gameFloorTransform->rotation = glm::angleAxis(glm::pi<float>() / 8, glm::vec3(0.0f, 0, 1.0f));
            gameFloorTransform->scale = glm::vec3(100.0f, 1.0f, 100.0f);
            gameFloorRigidBody->bodyType = eStaticBody;
            gameFloorRigidBody->scale = { 200.0f, 1.0f, 200.0f };
            gameFloor.setComponent(*AssetFetcher::meshFromPath("res/Models/Cube.obj"));
            gameFloorMesh->setPrimaryTexture("res/Textures/white.png");

            for (int i = 0; i < 5; i++) {
                Object conveyor = m_gameScene.createObject();
                auto conveyorTransform = conveyor.addComponent<TransformComponent>();
                auto conveyorMesh = conveyor.addComponent<Mesh>();
                auto conveyorRigidBody = conveyor.addComponent<RigidBodyComponent>();

                conveyorTransform->position = glm::vec3(0.0f, 0.0f, 25 * i);
                conveyor.setComponent(*AssetFetcher::meshFromPath("res/Models/Conveyor.obj"));
                conveyorMesh->setPrimaryTexture("res/Textures/white.png");
                conveyorRigidBody->bodyType = eStaticBody;
                conveyorRigidBody->scale = { 25.0f, 3.0f, 12.5f };
            }

            double startTime = glfwGetTime();
            int frames = 0;
            
            Clock physicsClock;

            // While the window is open, i.e. the operating system has not requested for it to be closed.
            while (window.isOpen()) {

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
                    if (!rigidBody->body) {
                        q3BodyDef bodyDef;

                        //bodyDef.userData = reinterpret_cast<void*>(entity);

                        auto rotAxis = glm::axis(transform->rotation);
                        
                        bodyDef.position = { transform->position.x, transform->position.y, transform->position.z };
                        bodyDef.axis = { rotAxis.x, rotAxis.y, rotAxis.z };
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
                if (double time = glfwGetTime(); time - startTime > 1.0) {
                    window.setTitle(std::format("EcoSort ({} FPS)", frames).c_str());
                    frames = 0;
                    startTime = time;
                }
            }
        }

        // Clean up GLFW
        glfwTerminate();
    }
}
