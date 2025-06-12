#include "Object.h"

#include "Scene.h"

namespace EcoSort {

    Object::Object(Scene& scene) : m_scene(scene), m_entityID(scene.m_registry.createEntity()) {}

    
    
}
