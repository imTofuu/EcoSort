#pragma once

#include <string>

namespace EcoSort {

    class Logger {
    public:

        explicit Logger(const char* name) : m_name(name) {}

        template<typename... Args>
        void assert(bool condition, const std::string& message, Args... args);
        template<typename... Args>
        void assert(bool condition, const char* message, Args... args);
        template<typename... Args>
        void weakAssert(bool condition, const std::string& message, Args... args);
        template<typename... Args>
        void weakAssert(bool condition, const char* message, Args... args);

        template<typename... Args>
        void debug(const std::string& message, Args... args) { debug(message.c_str(), args...); }
        template<typename... Args>
        void debug(const char* message, Args... args);
        template<typename... Args>
        void info(const std::string& message, Args... args) { info(message.c_str(), args...); }
        template<typename... Args>
        void info(const char* message, Args... args);
        template<typename... Args>
        void warn(const std::string& message, Args... args) { warn(message.c_str(), args...); }
        template<typename... Args>
        void warn(const char* message, Args... args);
        template<typename... Args>
        void error(const std::string& message, Args... args) { error(message.c_str(), args...); }
        template<typename... Args>
        void error(const char* message, Args... args);
        
    private:

        const char* m_name;
        
    };
    
}

#include "Logger.tpp"