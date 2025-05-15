#pragma once

#include <GLFW/glfw3.h>

#include "Interface.h"

namespace RecyclingGame {

    class Window {
    public:
        
        Window(const char* name, int width, int height);
        ~Window();

        void update();

        [[nodiscard]] bool isOpen() const { return !glfwWindowShouldClose(m_window); }
        
        [[nodiscard]] Interface& getInterface() { return m_interface; }

    private:

        GLFWwindow* m_window;

        Interface m_interface;
        
    };
    
}
