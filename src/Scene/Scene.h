#pragma once

#include <BOO/BOO.h>

namespace EcoSort {
    class Object;

    class Scene {
    public:
        Object createObject();

        template<typename... T>
        BOO::QueryResult<T...> findAll() { return m_registry.queryAll<T...>(); }

        template<typename... T>
        BOO::QueryResult<T...> findAny() { return m_registry.queryAny<T...>(); }

        template<typename... T>
        BOO::QueryResult<T...> findMatch() { return m_registry.queryMatch<T...>(); }

    private:
        friend class Object;
        BOO::Registry m_registry;
    };
}
