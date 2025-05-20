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

        // Create a new scope so the window will be destroyed once the main loop has finished.
        {
            Window window("RecyclingGame", 500, 500);

            // Load the OpenGL symbols using glad. This is required, otherwise all the OpenGL function pointers will be
            // null. The symbols are loaded here because glfwGetProcAddress requires a context to be current on the
            // main thread, which is done in the Window.
            gladLoadGL(glfwGetProcAddress);
            
            float vertices[] = {
                -0.5f, -0.5f, 0,
                0.5f, -0.5f, 0,
                0.5f, 0.5f, 0,
                0.5f, 0.5f, 0,
                -0.5f, 0.5f, 0,
                -0.5f, -0.5f, 0,
            };
            
            unsigned int vbo;
            glGenBuffers(1, &vbo);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            unsigned int vao;
            glGenVertexArrays(1, &vao);

            glBindVertexArray(vao);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            unsigned int vshader, fshader;
            vshader = glCreateShader(GL_VERTEX_SHADER);
            fshader = glCreateShader(GL_FRAGMENT_SHADER);

            const char* vshaderSource = "#version 410 core\n"
            "\n"
            "in vec3 a_position;\n"
            "void main() {\n"
            "   gl_Position = vec4(a_position, 1.0f);\n"
            "}\0";

            const char* fshaderSource = "#version 410 core\n"
            "\n"
            "out vec4 fragColour;\n"
            "void main() {\n"
            "   fragColour = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
            "}\0";

            glShaderSource(vshader, 1, &vshaderSource, nullptr);
            glCompileShader(vshader);
            glShaderSource(fshader, 1, &fshaderSource, nullptr);
            glCompileShader(fshader);

            int success;
            char infoLog[512];
            glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vshader, 512, nullptr, infoLog);
                m_logger.error("Vertex shader compilation failed: {}", infoLog);
            }
            glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fshader, 512, nullptr, infoLog);
                m_logger.error("Fragment shader compilation failed: {}", infoLog);
            }

            m_logger.info("{}", glGetError());

            unsigned int shaderProgram = glCreateProgram();
            
            glAttachShader(shaderProgram, vshader);
            glAttachShader(shaderProgram, fshader);
            glLinkProgram(shaderProgram);

            glUseProgram(shaderProgram);            

            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
                m_logger.error("Shader program linking failed: {}", infoLog);
            }

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

            // While the window is open, i.e., the operating system has not requested for it to be closed.
            while (window.isOpen()) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glDrawArrays(GL_TRIANGLES, 0, 6);

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
