#pragma once

#include "Scene.h"

namespace EcoSort {

    class Scene;

    class Object {
    public:

        explicit Object(Scene& scene);

        template<typename T>
        BOO::ComponentRef<T> addComponent() { return m_scene.m_registry.addComponentToEntity<T>(m_entityID); }
        template<typename T>
        BOO::ComponentRef<T> getComponent() { return m_scene.m_registry.getComponentFromEntity<T>(m_entityID); }
        template<typename T>
        void removeComponent() { m_scene.m_registry.removeComponentFromEntity<T>(m_entityID); }
        template<typename T>
        bool hasComponent() const { return m_scene.m_registry.entityHasComponent<T>(m_entityID); }
        template<typename T>
        void setComponent(const T& component) { m_scene.m_registry.setComponentOnEntity<T>(m_entityID, component); }

    private:

        Scene& m_scene;
        BOO::EntityID m_entityID;
        
    };
    
}