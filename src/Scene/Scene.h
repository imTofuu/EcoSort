#pragma once

#include <BOO/BOO.h>

namespace EcoSort {

    class Object;

    class Scene {
        
    public:

        Object createObject();

    private:
        
        friend class Object;
        BOO::Registry m_registry;
        
    };
    
}