#pragma once
#include "Interface/Logger.h"

namespace RecyclingGame {

    class Game {
    public:
        
        void run();

        [[nodiscard]] Logger& getLogger() { return m_logger; }
        [[nodiscard]] static Game* getInstance() { return s_instance; }

        static void setInstance(Game* instance) { s_instance = instance; }
    

    private:

        static Game* s_instance;

        Logger m_logger = Logger("RecyclingGame");
        
    };
    
}
