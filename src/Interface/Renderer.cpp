#include "Renderer.h"

#include "AssetFetcher.h"
#include "Game.h"
#include "Graphics/Mesh.h"
#include "Scene/Components.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GLFW/glfw3.h>

namespace EcoSort {

    Renderer::Renderer(int width, int height)
        : m_width(width), m_height(height), m_geometryTarget(width, height),
          m_lightingTarget(width, height), m_guiTarget(width, height),
          m_finalTarget(width, height) {

        m_geometryTarget.addAttachment({
            TextureType::COLOUR, DataType::FLOAT, false
        }); // gPositions
        m_geometryTarget.addAttachment({
            TextureType::COLOUR, DataType::FLOAT, false
        }); // gNormals
        m_geometryTarget.addAttachment({
            TextureType::COLOUR, DataType::UNSIGNED_BYTE, true
        }); // gAlbedos
        m_geometryTarget.addAttachment({
            TextureType::DEPTH, DataType::FLOAT, false
        }); // gDepth

        m_lightingTarget.addAttachment({
            TextureType::COLOUR, DataType::UNSIGNED_BYTE, true
        }); // lightingTexture

        m_guiTarget.addAttachment({
        TextureType::COLOUR, DataType::UNSIGNED_BYTE, true
        }); // guiTexture
        m_guiTarget.addAttachment({
            TextureType::DEPTH, DataType::FLOAT, false
        }); // guiDepth

        m_finalTarget.addAttachment({
        TextureType::COLOUR, DataType::UNSIGNED_BYTE, true
        }); // finalTexture
        m_finalTarget.addAttachment({
            TextureType::DEPTH, DataType::FLOAT, false
        }); // finalDepth

        // These can be safely marked for deletion once linked to the shader program.
        Shader gBufferVertShader("res/Shaders/Scene/Deferred/gbuffer.vert", ShaderType::VERT),
               gBufferFragShader("res/Shaders/Scene/Deferred/gbuffer.frag", ShaderType::FRAG),
        
               lightingVertShader("res/Shaders/Scene/Deferred/lighting.vert", ShaderType::VERT),
               lightingFragShader("res/Shaders/Scene/Deferred/lighting.frag", ShaderType::FRAG),

               guiVertShader("res/Shaders/GUI/gui.vert", ShaderType::VERT),
               guiFragShader("res/Shaders/GUI/gui.frag", ShaderType::FRAG),
        
               finalVertShader("res/Shaders/Scene/Deferred/final.vert", ShaderType::VERT),
               finalFragShader("res/Shaders/Scene/Deferred/final.frag", ShaderType::FRAG),

               debugLightVertShader("res/Shaders/Debug/showlights.vert", ShaderType::VERT),
               debugLightFragShader("res/Shaders/Debug/showlights.frag", ShaderType::FRAG);

        m_geometryProgram.attachShader(gBufferVertShader);
        m_geometryProgram.attachShader(gBufferFragShader);

        m_lightingProgram.attachShader(lightingVertShader);
        m_lightingProgram.attachShader(lightingFragShader);

        m_guiProgram.attachShader(guiVertShader);
        m_guiProgram.attachShader(guiFragShader);

        m_finalProgram.attachShader(finalVertShader);
        m_finalProgram.attachShader(finalFragShader);

        m_debugLightProgram.attachShader(debugLightVertShader);
        m_debugLightProgram.attachShader(debugLightFragShader);

        m_geometryProgram.setInt("u_primaryTexture", 0);

        m_lightingProgram.setInt("u_gPositions", 0);
        m_lightingProgram.setInt("u_gNormals", 1);
        m_lightingProgram.setInt("u_gAlbedos", 2);

        m_guiProgram.setInt("u_image", 0);

        m_finalProgram.setInt("u_screen", 0);

        m_screenMesh = *AssetFetcher::meshFromPath("res/Models/Fullscreen.obj");
        m_debugLightMesh = *AssetFetcher::meshFromPath("res/Models/Cube.obj");

        m_whiteTexture.setData("res/Textures/white.png");

        glClearColor(0, 0, 0, 1);

        // Tell OpenGL to only call the fragment shader for front faces, decided
        // by the winding of their vertices
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        // Enable depth testing so that the fragment shader is not called for
        // fragments that are behind other fragments.
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glfwSwapInterval(1);
        
    }

    void Renderer::resize(int width, int height) {
        m_width = width;
        m_height = height;
        
        m_geometryTarget.resize(width, height);
        m_lightingTarget.resize(width, height);
        m_finalTarget.resize(width, height);
    }
    
    void Renderer::renderScene(Scene& scene, RenderTarget* renderTarget) {

        // GEOMETRY PASS

        glEnable(GL_DEPTH_TEST);

        m_geometryTarget.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_geometryProgram.use();

        CameraComponent* camera = nullptr;
        TransformComponent* cameraTransform = nullptr;

        for (auto& [ cameraComp, cameraTransformComp ] : scene.findAll<CameraComponent, TransformComponent>()) {
            camera = cameraComp;
            cameraTransform = cameraTransformComp;
            break;
        }

        if (!camera || !cameraTransform) {
            LOGGER.warn("No camera found in the scene!");
            return;
        }

        auto projection = glm::perspective(camera->fov, 
            static_cast<float>(m_width) / static_cast<float>(m_height),
            0.1f, 100.0f);
        auto view = cameraTransform->getTransformation();

        m_geometryProgram.setMat4("u_projection", glm::value_ptr(projection));
        m_geometryProgram.setMat4("u_view", glm::value_ptr(view));

        for (auto& [ mesh, transform ] : scene.findAll<Mesh, TransformComponent>()) {

            auto model = transform->getTransformation();
            m_geometryProgram.setMat4("u_model", glm::value_ptr(model));

            auto normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
            m_geometryProgram.setMat3("u_normalMatrix", glm::value_ptr(normalMatrix));

            mesh->draw();
        }

        glDisable(GL_DEPTH_TEST);

        // LIGHTING PASS

        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        m_lightingTarget.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_lightingProgram.use();

        m_geometryTarget.use();

        m_lightingProgram.setInt("u_light.lightType", 4);

        m_screenMesh.draw();

        for (auto& [ light, transform ] : scene.findAll<LightComponent, TransformComponent>()) {

            m_lightingProgram.setInt("u_light.lightType", static_cast<int>(light->type));
            m_lightingProgram.setFloats("u_light.position", glm::value_ptr(transform->position), 3);
            
            auto defaultLightDirection = glm::vec3(0.0f, 1.0f, 0.0f);
            auto dir = transform->rotation * defaultLightDirection;
            
            m_lightingProgram.setFloats("u_light.direction", glm::value_ptr(dir), 3);
            m_lightingProgram.setFloats("u_light.colour", glm::value_ptr(light->colour), 3);
            m_lightingProgram.setFloat("u_light.distance", light->distance);
            
            m_screenMesh.draw();
            
        }

        glDisable(GL_BLEND);

        // GUI PASS

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0, 0, 0, 0);

        m_guiTarget.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_guiProgram.use();

        auto guiProjection = glm::ortho(
            0.0f, static_cast<float>(m_width),
            static_cast<float>(m_height), 0.0f,
            0.0f, 100.0f
            );

        m_guiProgram.setMat4("u_projection", glm::value_ptr(guiProjection));

        for (auto& [ gui, transform ] : scene.findAll<GUIComponent, Transform2DComponent>()) {

            TransformComponent scaledTransform;

            scaledTransform.position = glm::vec3(
                transform->position.offset.x + (transform->position.scale.x * m_width),
                transform->position.offset.y + (transform->position.scale.y * m_height),
                transform->zIndex
            );
            scaledTransform.scale = glm::vec3(
                transform->size.offset.x + (transform->size.scale.x * m_width),
                transform->size.offset.y + (transform->size.scale.y * m_height),
                1.0f
            );

            auto model = scaledTransform.getTransformation();
            m_guiProgram.setMat4("u_model", glm::value_ptr(model));

            if (gui->image) {
                Texture::setUnit(0);
                gui->image->bind();
            } else {
                Texture::setUnit(0);
                m_whiteTexture.bind();
            }

            m_guiProgram.setFloats("u_colour", glm::value_ptr(gui->colour), 4);

            // Since screenMesh is a generic quad, it can be used for this too.
            m_screenMesh.draw();
        }

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);

        glClearColor(0, 0, 0, 1);

        // FINAL PASS

        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        m_finalTarget.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_finalProgram.use();

        m_lightingTarget.use();
        m_screenMesh.draw();

#ifdef RG_DEBUG_SHOW_LIGHTS

        // DEBUG LIGHTS SUBPASS

        glEnable(GL_DEPTH_TEST);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_geometryTarget.m_framebuffer.m_handle);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_finalTarget.m_framebuffer.m_handle);

        glBlitFramebuffer(
            0, 0, m_width, m_height,
            0, 0, m_width, m_height,
            GL_DEPTH_BUFFER_BIT,
            GL_NEAREST
            );
        
        m_finalTarget.bind();

        m_debugLightProgram.use();

        m_debugLightProgram.setMat4("u_projection", glm::value_ptr(projection));
        m_debugLightProgram.setMat4("u_view", glm::value_ptr(view));

        for (auto& [ light, transform ] : scene.findAll<LightComponent, TransformComponent>()) {

            transform->scale = glm::vec3(0.1f);
            if (light->type == LightComponent::LightType::DIRECTIONAL)
                transform->scale.y *= 3.0f;

            auto model = transform->getTransformation();
            m_debugLightProgram.setMat4("u_model", glm::value_ptr(model));

            m_debugLightProgram.setFloats("u_lightColour", glm::value_ptr(light->colour), 3);

            m_debugLightMesh.draw();
            
        }

        glDisable(GL_DEPTH_TEST);
        
#endif

        m_finalProgram.use();

        m_guiTarget.use();
        m_screenMesh.draw();

        glDisable(GL_BLEND);
        
        blit(m_finalTarget, renderTarget);
        
    }

    void Renderer::blit(const RenderTarget& src, RenderTarget* dst) {
        
        glBindFramebuffer(
            GL_READ_FRAMEBUFFER,
            src.m_framebuffer.m_handle
            );
        glBindFramebuffer(
            GL_DRAW_FRAMEBUFFER,
            dst ? dst->m_framebuffer.m_handle : 0
            );

        glBlitFramebuffer(
            0, 0, m_width, m_height,
            0, 0, dst ? dst->m_width : m_width, dst ? dst->m_height : m_height,
            GL_COLOR_BUFFER_BIT,
            GL_LINEAR
        );
    }
    
}
