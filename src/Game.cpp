#include "Game.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Interface/Window.h"
#include "Scene/Object.h""

namespace RecyclingGame {

    Game* Game::s_instance = nullptr;

    void glfwErrorCallback(int error, const char* description) {
        static Logger logger("GLFW");
        logger.error("Error {}: {}", error, description);
    }

    void Game::run() {

        // If GLFW has an error, it will call this function where I log the error.
        glfwSetErrorCallback(glfwErrorCallback);

        int result;

        // Initialise GLFW so a window can be created.
        result = glfwInit();
        m_logger.assert(result, "Failed to initialize GLFW");

        // Specify the version of OpenGL that will be used as a window hint. The window hints will apply to all windows
        // created after they are set. This project uses GL 4.1 core, since it is the most recent version supported by
        // macOS.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        Object object = m_scene.createObject();
        int* intcomp = object.addComponent<int>();

        // Create a new scope so the window will be destroyed once the main loop has finished.
        {
            Window window("RecyclingGame", 500, 500);

            // Load the OpenGL symbols using glad. This is required, otherwise all the OpenGL function pointers will be
            // null. The symbols are loaded here because glfwGetProcAddress requires a context to be current on the
            // main thread, which is done in the Window.
            gladLoadGL(glfwGetProcAddress);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

            // While the window is open, i.e., the operating system has not requested for it to be closed.
            while (window.isOpen()) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                *intcomp ++;

                // Poll events in GLFW, which will handle OS events and user interfaces, such as the keyboard and mouse.
                glfwPollEvents();

                // Swap the buffers of the window.
                window.update();
            }
        }

        // Clean up GLFW
        glfwTerminate();
    }
    
}
