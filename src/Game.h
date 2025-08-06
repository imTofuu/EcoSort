#pragma once

#include "Interface/Logger.h"
#include "Interface/Window.h"
#include "q3.h"
#include "Scene/Scene.h"

#define LOGGER EcoSort::Game::getInstance()->getLogger()

namespace EcoSort {

    class Game {
    public:
        
        void run();

        Logger& getLogger() { return m_logger; }
        static Game* getInstance() { return s_instance; }
        Scene& getActiveScene() { return m_activeScene; }

        static void setInstance(Game* instance) { s_instance = instance; }

    private:

        static Game* s_instance;

        Scene m_gameScene,
              m_menuScene;

        Scene& m_activeScene = m_menuScene;
        q3Scene m_physicsScene = q3Scene(1.0f / 60.0f);

        Logger m_logger = Logger("EcoSort");
        
    };
    
}
