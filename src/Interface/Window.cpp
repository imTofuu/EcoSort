#include "Window.h"

#include "Renderer.h"

#include <Game.h>
#include <glad/gl.h>

namespace EcoSort {

    void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

        windowPtr->getInterface().handleKeyAction(action, key);
    }

    void glfwMouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

        windowPtr->getInterface().handleMouseMove(xpos, ypos);
    }

    void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

        windowPtr->getInterface().handleMouseButton(action, button);
    }

    void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {
        auto* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

        int w, h;
        windowPtr->getFramebufferSize(&w, &h);
        
        windowPtr->getRenderer()->resize(w, h);
    }
    
    Window::Window(const char* name, int width, int height) {

        m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);

        // Sets the OpenGL context in the window that was just created as current on this thread. This means that any
        // OpenGL calls on this thread will affect that context.
        glfwMakeContextCurrent(m_window);
        
        glfwSetWindowUserPointer(m_window, this);

        glfwSetKeyCallback(m_window, glfwKeyCallback);
        glfwSetCursorPosCallback(m_window, glfwMouseMoveCallback);
        glfwSetMouseButtonCallback(m_window, glfwMouseButtonCallback);
        glfwSetWindowSizeCallback(m_window, glfwWindowSizeCallback);

        int result = gladLoadGL(glfwGetProcAddress);
        LOGGER.assert(result, "Failed to initialize GLAD");
        LOGGER.debug("OpenGL Version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));

        int w, h;
        getFramebufferSize(&w, &h);
        
        m_renderer = new Renderer(w, h);
    }

    Window::~Window() {
        glfwDestroyWindow(m_window);
    }
    
    void Window::update() {

        m_renderer->renderScene(Game::getInstance()->getScene(), nullptr);
        
        glfwSwapBuffers(m_window);
    }
    
}