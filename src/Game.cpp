#include "Game.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace RecyclingGame {

    void Game::run() {

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(500, 500, "erghtyrgrefwr", nullptr, nullptr);

        glfwMakeContextCurrent(window);
    
        gladLoadGL(glfwGetProcAddress);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT);
        
            glfwPollEvents();

            glfwSwapBuffers(window);
        }

        glfwDestroyWindow(window);

        glfwTerminate();
        
    }
    
}