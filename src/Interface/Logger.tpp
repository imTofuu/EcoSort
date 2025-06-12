
#include <csignal>
#include <iostream>

namespace EcoSort {

    template<typename... Args>
    void Logger::assert(bool condition, const std::string& message, Args... args) {
        assert(condition, message.c_str(), args...);
    }

    template<typename... Args>
    void Logger::assert(bool condition, const char* message, Args... args) {
        if (condition) return;
        error("Failed assertion: " + std::string(message), args...);
    }

    template<typename... Args>
    void Logger::weakAssert(bool condition, const std::string& message, Args... args) {
        weakAssert(condition, message.c_str(), args...);
    }

    template<typename... Args>
    void Logger::weakAssert(bool condition, const char* message, Args... args) {
        if (condition) return;
        warn("Weak assertion failed: " + std::string(message), args...);
    }

    template<typename... Args>
    void Logger::debug(const char* message, Args... args) {
#ifndef RG_DEBUG
        return;
#endif
        std::cout << std::format("\e[0;32m[DEBUG, {}] ", m_name);
        std::cout << std::vformat(message, std::make_format_args(args...)) << "\e[0m" << std::endl;
    }

    template<typename... Args>
    void Logger::info(const char* message, Args... args) {
        std::cout << std::format("[INFO, {}] ", m_name);
        std::cout << std::vformat(message, std::make_format_args(args...)) << std::endl;
    }

    template<typename... Args>
    void Logger::warn(const char* message, Args... args) {
        std::cout << std::format("\e[0;33m[WARN, {}] ", m_name);
        std::cout << std::vformat(message, std::make_format_args(args...)) << "\e[0m" << std::endl;
    }

    template<typename... Args>
    void Logger::error(const char* message, Args... args) {
        std::cout << std::format("\e[0;31m[ERROR, {}] ", m_name);
        std::cout << std::vformat(message, std::make_format_args(args...)) << "\e[0m" << std::endl;
        raise(SIGABRT);
    }
    
}
