#pragma once

#include <memory>

#include "Graphics/Mesh.h"

namespace RecyclingGame {

    class AssetFetcher {
    public:
        
        static std::shared_ptr<Mesh> meshFromPath(const char* path);
        
    };
    
}
