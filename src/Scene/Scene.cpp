#include "Scene.h"

#include "Object.h"

namespace EcoSort {

    void Scene::removeObject(Object& object) {
        m_registry.destroyEntity(object.m_entityID);
    }

    
     Object Scene::createObject() {
         return Object(*this);
    }

    
}