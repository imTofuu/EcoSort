#pragma once

#include "Interface/Logger.h"
#include "Scene/Scene.h"

#define LOGGER EcoSort::Game::getInstance()->getLogger()

namespace EcoSort {

    class Game {
    public:
        
        void run();

        [[nodiscard]] Logger& getLogger() { return m_logger; }
        [[nodiscard]] static Game* getInstance() { return s_instance; }

        static void setInstance(Game* instance) { s_instance = instance; }
    

    private:

        static Game* s_instance;

        Scene m_scene;

        Logger m_logger = Logger("EcoSort");
        
    };
    
}
