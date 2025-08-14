#include "Object.h"

#include "Scene.h"

namespace EcoSort {

    Object::Object(Scene& scene) : m_scene(scene), m_entityID(scene.m_registry.createEntity()) {}

    void Object::destroy() {
        m_scene.removeObject(*this);
    }

    bool Object::valid() {
        return m_scene.m_registry.entityExists(m_entityID);
    }
    
}
