#include "Game.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "AssetFetcher.h"
#include "Interface/Window.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Texture.h"

namespace EcoSort {

    Game* Game::s_instance = nullptr;

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
            result = gladLoadGL(glfwGetProcAddress);
            m_logger.assert(result, "Failed to initialize GLAD");
            m_logger.debug("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

            // Tell OpenGL to only call the fragment shader for front faces, decided by the winding of their vertices
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);

            // Enable depth testing so that the fragment shader not called for fragments that are behind other
            // fragments.
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            // Tell OpenGL how to blend semi-transparent or fully transparent fragments with what is behind it.
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);

            std::shared_ptr<Mesh> mesh = AssetFetcher::meshFromPath("res/Models/Suzanne.obj");
            
            Shader vertexShader("res/Shaders/test.vert", ShaderType::VERT);
            Shader fragShader("res/Shaders/test.frag", ShaderType::FRAG);
            
            ShaderProgram shaderProgram;
            shaderProgram.attachShader(vertexShader);
            shaderProgram.attachShader(fragShader);
            
            shaderProgram.link();
            
            shaderProgram.use();

            shaderProgram.setInt("texture1", 0);
            shaderProgram.setInt("texture2", 1);

            Texture texture;
            texture.bind(0);
            texture.setData("res/Textures/img.png");

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            
            glfwSwapInterval(1);

            double startTime = glfwGetTime();
            int frames = 0;
            
            // While the window is open, i.e. the operating system has not requested for it to be closed.
            while (window.isOpen()) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                
                mesh->draw();

                // Poll events in GLFW, which will handle OS events and user interfaces, such as the keyboard and mouse.
                glfwPollEvents();

                // Swap the buffers of the window.
                window.update();

                frames++;

                // Reset and display the fps counter if a second has passed
                if (double time = glfwGetTime(); time - startTime > 1.0) {
                    window.setTitle(std::format("Recycling Game ({} FPS)", frames).c_str());
                    frames = 0;
                    startTime = time;
                }
                
            }
        }

        // Clean up GLFW
        glfwTerminate();
    }
    
}
