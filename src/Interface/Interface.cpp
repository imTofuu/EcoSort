#include "Interface.h"

namespace EcoSort {
    
    void Interface::handleKeyAction(int action, int key) {
        if (action == GLFW_PRESS) {
            m_enabledKeys.insert(static_cast<Key>(key));
        } else if (action == GLFW_RELEASE) {
            m_enabledKeys.erase(static_cast<Key>(key));
        }
    }

    void Interface::handleMouseMove(double xpos, double ypos) {
        m_mouseX = xpos;
        m_mouseY = ypos;
    }

    void Interface::handleMouseButton(int action, int button) {
        if (action == GLFW_PRESS) {
            m_enabledMouseButtons.insert(static_cast<MouseButton>(button));
        } else if (action == GLFW_RELEASE) {
            m_enabledMouseButtons.erase(static_cast<MouseButton>(button));
        }
    }

    bool Interface::getKeyEnabledState(Key key) const {
        return m_enabledKeys.contains(key);
    }

    const std::set<Key>& Interface::getEnabledKeys() const {
        return m_enabledKeys;
    }

    bool Interface::getMouseButtonEnabledState(MouseButton button) const {
        return m_enabledMouseButtons.contains(button);
    }

    const std::set<MouseButton>& Interface::getEnabledMouseButtons() const {
        return m_enabledMouseButtons;
    }

    const char* Interface::getKeyName(Key key) {
        switch (key) {
            case Key::ESCAPE: return "Escape";
            case Key::F1: return "F1";
            case Key::F2: return "F2";
            case Key::F3: return "F3";
            case Key::F4: return "F4";
            case Key::F5: return "F5";
            case Key::F6: return "F6";
            case Key::F7: return "F7";
            case Key::F8: return "F8";
            case Key::F9: return "F9";
            case Key::F10: return "F10";
            case Key::F11: return "F11";
            case Key::F12: return "F12";

            case Key::TILDE: return "Tilde";
            case Key::ONE: return "1";
            case Key::TWO: return "2";
            case Key::THREE: return "3";
            case Key::FOUR: return "4";
            case Key::FIVE: return "5";
            case Key::SIX: return "6";
            case Key::SEVEN: return "7";
            case Key::EIGHT: return "8";
            case Key::NINE: return "9";
            case Key::ZERO: return "0";
            case Key::MINUS: return "Minus";
            case Key::EQUALS: return "Equals";
            case Key::BACKSPACE: return "Backspace";

            case Key::TAB: return "Tab";
            case Key::Q: return "Q";
            case Key::W: return "W";
            case Key::E: return "E";
            case Key::R: return "R";
            case Key::T: return "T";
            case Key::Y: return "Y";
            case Key::I: return "I";
            case Key::U: return "U";
            case Key::O: return "O";
            case Key::P: return "P";
            case Key::LEFT_BRACKET: return "Left Bracket";
            case Key::RIGHT_BRACKET: return "Right Bracket";
            case Key::BACKSLASH: return "Backslash";

            case Key::CAPS: return "Caps Lock";
            case Key::A: return "A";
            case Key::S: return "S";
            case Key::D: return "D";
            case Key::F: return "F";
            case Key::G: return "G";
            case Key::H: return "H";
            case Key::J: return "J";
            case Key::K: return "K";
            case Key::L: return "L";
            case Key::SEMICOLON: return "Semicolon";
            case Key::APOSTROPHE: return "Apostrophe";
            case Key::ENTER: return "Enter";

            case Key::LEFT_SHIFT: return "Left Shift";
            case Key::Z: return "Z";
            case Key::X: return "X";
            case Key::C: return "C";
            case Key::V: return "V";
            case Key::B: return "B";
            case Key::N: return "N";
            case Key::M: return "M";
            case Key::COMMA: return "Comma";
            case Key::PERIOD: return "Period";
            case Key::SLASH: return "Slash";
            case Key::RIGHT_SHIFT: return "Right Shift";

            case Key::LEFT_CONTROL: return "Left Control";
            case Key::LEFT_ALT: return "Left Alt";
            case Key::SPACE: return "Space";
            case Key::RIGHT_ALT: return "Right Alt";
            case Key::RIGHT_CONTROL: return "Right Control";
            case Key::LEFT: return "Left Arrow";
            case Key::UP: return "Up Arrow";
            case Key::RIGHT: return "Right Arrow";
            case Key::DOWN: return "Down Arrow";
            default: return "Unknown Key";
        }
    }

    const char* Interface::getMouseButtonName(MouseButton button) {
        switch (button) {
            case MouseButton::LEFT: return "Left";
            case MouseButton::MIDDLE: return "Middle";
            case MouseButton::RIGHT: return "Right";
            case MouseButton::BACK: return "Back";
            case MouseButton::FORWARD: return "Forward";
            default: return "Unknown mouse button";
        }
    }

}
