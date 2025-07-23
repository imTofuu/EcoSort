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

    void glfwErrorCallback(int error, const char *description) {
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
        m_logger.assert(result, "Failed to initialize GLFW");
        m_logger.debug("Initialised GLFW");

        // Specify the version of OpenGL that will be used as a window hint. The window hints will apply to all windows
        // created after they are set. This project uses GL 4.1 core, since it is the most recent version supported by
        // macOS.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a new scope so the window will be destroyed once the main loop has finished.
        {
            Window window("EcoSort", 500, 500);
            m_logger.info("Initialised window");

            // Load the OpenGL symbols using glad. This is required, otherwise all the OpenGL function pointers will be
            // null. The symbols are loaded here because glfwGetProcAddress requires a context to be current on the
            // main thread, which is done in the Window initialisation.
            /*result = gladLoadGL(glfwGetProcAddress);
            m_logger.assert(result, "Failed to initialize GLAD");
            m_logger.debug("OpenGL Version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));*/

            int vpWidth, vpHeight;
            window.getFramebufferSize(&vpWidth, &vpHeight);

            Object camera = m_scene.createObject();
            auto cameracomp = camera.addComponent<CameraComponent>();
            auto cameraTransform = camera.addComponent<TransformComponent>();

            cameraTransform->position = glm::vec3(0.0f, 0.0f, -10.0f);

            Object object = m_scene.createObject();
            auto objectTransform = object.addComponent<TransformComponent>();
            objectTransform->scale.x = 1;

            // My library is bad, dumb workaround
            auto mesh = object.addComponent<Mesh>();
            object.setComponent(*AssetFetcher::meshFromPath("res/Models/Suzanne.obj"));
            mesh->setPrimaryTexture("res/Textures/img.png");

            double startTime = glfwGetTime();
            int frames = 0;

            int f = 0;

            // While the window is open, i.e. the operating system has not requested for it to be closed.
            while (window.isOpen()) {

                objectTransform->position.x = glm::sin((f * 2 * glm::pi<float>()) / 1000) * 2.0f;
                objectTransform->position.z = glm::cos((f * 2 * glm::pi<float>()) / 1000) * 2.0f;

                f++;

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
