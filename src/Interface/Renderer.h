#pragma once

#include "Graphics/Mesh.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/ShaderProgram.h"
#include "Scene/Components.h"
#include "Scene/Scene.h"

namespace EcoSort {

    class Renderer {
    public:

        Renderer(int width, int height);

        void resize(int width, int height);

        void renderScene(Scene& scene, RenderTarget* renderTarget);

        // dst can be null, will blit to the screen.
        void blit(const RenderTarget& src, RenderTarget* dst);

        TransformComponent
        getAbsoluteTransform2D(const Transform2DComponent &transform);
        static TransformComponent getRelativeTransform2D(const Transform2DComponent& child, const TransformComponent& parent);

    private:

        int m_width,
            m_height;

        RenderTarget m_geometryTarget,
                     m_lightingTarget,
                     m_guiTarget,
                     m_finalTarget;

        ShaderProgram m_geometryProgram,
                      m_lightingProgram,
                      m_guiProgram,
                      m_finalProgram,

                      m_debugLightProgram;
        
        Mesh m_screenMesh,
             m_guiQuad,
        
             m_debugLightMesh;

        Texture m_whiteTexture;
        
    };
    
}
