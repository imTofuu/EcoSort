#pragma once

#include <memory>

#include "Graphics/Mesh.h"

namespace EcoSort {

    class AssetFetcher {
    public:
        
        static std::shared_ptr<Mesh> meshFromPath(const char* path);
        
    };
    
}
