#include "Scene.h"

#include "Object.h"

namespace EcoSort {
    
     Object Scene::createObject() {
         return Object(*this);
    }

    
}