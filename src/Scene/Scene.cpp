#include "Scene.h"

#include "Object.h"

namespace RecyclingGame {

    Object Scene::createObject() {
        return Object(*this);
    }

    
}