#include "Game.h"

#include "glm/gtc/type_ptr.hpp"
#include <GLFW/glfw3.h>

#include "AssetFetcher.h"
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

            Object menuCamera = m_menuScene.createObject();
            auto menuCameraComp = menuCamera.addComponent<CameraComponent>();
            auto menuCameraTransform = menuCamera.addComponent<TransformComponent>();

            menuCameraTransform->position = glm::vec3(-3.0f, 2.0f, -10.0f);
            menuCameraTransform->rotation = glm::quatLookAt(-glm::normalize(menuCameraTransform->position), glm::vec3(0.0f, 1.0f, 0.0f));

            Object object = m_menuScene.createObject();
            auto objectTransform = object.addComponent<TransformComponent>();
            objectTransform->position = glm::vec3(0.0f, 0.0f, 0.0f);
            objectTransform->scale = glm::vec3(0.02f);

            // My library is bad, dumb workaround
            auto mesh = object.addComponent<Mesh>();
            object.setComponent(*AssetFetcher::meshFromPath("res/Models/StanfordDragon.obj"));
            mesh->setPrimaryTexture("res/Textures/white.png");

            Object light = m_menuScene.createObject();
            auto lightTransform = light.addComponent<TransformComponent>();
            auto lightComp = light.addComponent<LightComponent>();
            lightTransform->position = glm::vec3(0.5f, 1.0f, -1.0f);
            lightComp->colour = glm::vec3(1.0f, 0.0f, 0.0f);
            lightComp->distance = 12.0f;

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

            auto& [ playButton, playButtonTransform ] = menuListComp->guis.emplace_back();
            playButtonTransform.position = {
                { 0, 0 },
                { 0.0f, 0.0f }
            };
            playButtonTransform.size = {
                { 0, 0 },
                { 1, 0.2 }
            };
            playButton.image = std::make_shared<Texture>();
            playButton.image->setData("res/Textures/img.png");

            double startTime = glfwGetTime();
            int frames = 0;

            // While the window is open, i.e. the operating system has not requested for it to be closed.
            while (window.isOpen()) {

                Interface& interface = window.getInterface();

                auto mouseX = interface.getMouseX();
                auto mouseY = interface.getMouseY();
                auto mouseEnabled = interface.getMouseButtonEnabledState(MouseButton::LEFT);

                for (auto& [ guiFrame, frameTransform ] : m_activeScene.findAll<GUIFrameComponent, Transform2DComponent>()) {

                    TransformComponent absoluteFrameTransform = window.getRenderer()->getAbsoluteTransform2D(*frameTransform);

                    for (auto& [ gui, transform ] : guiFrame->guis) {

                        TransformComponent absoluteTransform = window.getRenderer()->getRelativeTransform2D(transform, absoluteFrameTransform);

                        gui.isHovered = mouseX >= absoluteTransform.position.x - (absoluteTransform.scale.x / 2) &&
                            mouseX <= absoluteTransform.position.x + (absoluteTransform.scale.x / 2) &&
                            mouseY >= absoluteTransform.position.y - (absoluteTransform.scale.y / 2) &&
                            mouseY <= absoluteTransform.position.y + (absoluteTransform.scale.y / 2);

                        gui.isClicked = mouseEnabled && gui.isHovered;
                        
                    }
                }

                // Poll events in GLFW, which will handle OS events and user interfaces, such as the keyboard and mouse.
                glfwPollEvents();

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
