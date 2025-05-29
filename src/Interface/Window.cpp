#include "Window.h"

#include <Game.h>

namespace RecyclingGame {

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
    
    Window::Window(const char* name, int width, int height) {

        m_window = glfwCreateWindow(width, height, name, nullptr, nullptr);

        // Sets the OpenGL context in the window that was just created as current on this thread. This means that any
        // OpenGL calls on this thread will affect that context.
        glfwMakeContextCurrent(m_window);
        
        glfwSetWindowUserPointer(m_window, this);

        glfwSetKeyCallback(m_window, glfwKeyCallback);

        glfwSetCursorPosCallback(m_window, glfwMouseMoveCallback);
        glfwSetMouseButtonCallback(m_window, glfwMouseButtonCallback);
    }

    Window::~Window() {
        glfwDestroyWindow(m_window);
    }

    void Window::update() {
        glfwSwapBuffers(m_window);
    }

    
}