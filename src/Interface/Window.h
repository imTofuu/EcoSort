#pragma once

#include <GLFW/glfw3.h>

#include "Interface.h"

namespace EcoSort {

    class Window {
    public:
        
        Window(const char* name, int width, int height);
        ~Window();

        void update();
        
        void setTitle(const char* title) { glfwSetWindowTitle(m_window, title); }

        void getSize(int* w, int* h) { glfwGetWindowSize(m_window, w, h); }
        void getFramebufferSize(int* w, int* h) { glfwGetFramebufferSize(m_window, w, h); }

        [[nodiscard]] bool isOpen() const { return !glfwWindowShouldClose(m_window); }
        
        [[nodiscard]] Interface& getInterface() { return m_interface; }

    private:

        GLFWwindow* m_window;

        Interface m_interface;
        
    };
    
}
