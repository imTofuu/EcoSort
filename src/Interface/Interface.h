#pragma once

#include <set>
#include <GLFW/glfw3.h>


namespace EcoSort {

    enum class Key {
        ESCAPE = GLFW_KEY_ESCAPE, F1 = GLFW_KEY_F1, F2 = GLFW_KEY_F2, F3 = GLFW_KEY_F3, F4 = GLFW_KEY_F4,
        F5 = GLFW_KEY_F5, F6 = GLFW_KEY_F6, F7 = GLFW_KEY_F7, F8 = GLFW_KEY_F8, F9 = GLFW_KEY_F9, F10 = GLFW_KEY_F10,
        F11 = GLFW_KEY_F11, F12 = GLFW_KEY_F12,

        TILDE = GLFW_KEY_GRAVE_ACCENT, ONE = GLFW_KEY_1, TWO = GLFW_KEY_2, THREE = GLFW_KEY_3, FOUR = GLFW_KEY_4,
        FIVE = GLFW_KEY_5, SIX = GLFW_KEY_6, SEVEN = GLFW_KEY_7, EIGHT = GLFW_KEY_8, NINE = GLFW_KEY_9,
        ZERO = GLFW_KEY_0, MINUS = GLFW_KEY_MINUS, EQUALS = GLFW_KEY_EQUAL, BACKSPACE = GLFW_KEY_BACKSPACE,
        
        TAB = GLFW_KEY_TAB, Q = GLFW_KEY_Q, W = GLFW_KEY_W, E = GLFW_KEY_E, R = GLFW_KEY_R, T = GLFW_KEY_T,
        Y = GLFW_KEY_Y, U = GLFW_KEY_U, I = GLFW_KEY_I, O = GLFW_KEY_O, P = GLFW_KEY_P,
        LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET, RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET, BACKSLASH = GLFW_KEY_BACKSLASH,
        
        CAPS = GLFW_KEY_CAPS_LOCK, A = GLFW_KEY_A, S = GLFW_KEY_S, D = GLFW_KEY_D, F = GLFW_KEY_F, G = GLFW_KEY_G,
        H = GLFW_KEY_H, J = GLFW_KEY_J, K = GLFW_KEY_K, L = GLFW_KEY_L, SEMICOLON = GLFW_KEY_SEMICOLON,
        APOSTROPHE = GLFW_KEY_APOSTROPHE, ENTER = GLFW_KEY_ENTER,

        LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT, Z = GLFW_KEY_Z, X = GLFW_KEY_X, C = GLFW_KEY_C, V = GLFW_KEY_V,
        B = GLFW_KEY_B, N = GLFW_KEY_N, M = GLFW_KEY_M, COMMA = GLFW_KEY_COMMA, PERIOD = GLFW_KEY_PERIOD,
        SLASH = GLFW_KEY_SLASH, RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,

        LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL, LEFT_ALT = GLFW_KEY_LEFT_ALT, SPACE = GLFW_KEY_SPACE,
        RIGHT_ALT = GLFW_KEY_RIGHT_ALT, RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL, LEFT = GLFW_KEY_LEFT, UP = GLFW_KEY_UP,
        RIGHT = GLFW_KEY_RIGHT, DOWN = GLFW_KEY_DOWN
        
    };

    enum class MouseButton {
        LEFT = GLFW_MOUSE_BUTTON_LEFT, MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE, RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
        BACK = GLFW_MOUSE_BUTTON_4, FORWARD = GLFW_MOUSE_BUTTON_5
    };

    class Interface {
    public:

        void handleKeyAction(int action, int key);
        void handleMouseMove(double xpos, double ypos);
        void handleMouseButton(int action, int button);

        [[nodiscard]] bool getKeyEnabledState(Key key) const;
        [[nodiscard]] const std::set<Key>& getEnabledKeys() const;

        [[nodiscard]] bool getMouseButtonEnabledState(MouseButton button) const;
        [[nodiscard]] const std::set<MouseButton>& getEnabledMouseButtons() const;

        [[nodiscard]] double getMouseX() const { return m_mouseX; }
        [[nodiscard]] double getMouseY() const { return m_mouseY; }
        
        [[nodiscard]] static const char* getKeyName(Key key);
        [[nodiscard]] static const char* getMouseButtonName(MouseButton button);

    private:

        std::set<Key> m_enabledKeys;
        std::set<MouseButton> m_enabledMouseButtons;

        double m_mouseX = 0.0;
        double m_mouseY = 0.0;
        
    };
    
}
