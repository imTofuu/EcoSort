#include "Game.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "AssetFetcher.h"
#include "Graphics/Framebuffer.h"
#include "Interface/Window.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Texture.h"
#include "Scene/Components.h"
#include "Scene/Object.h"

#undef assert

namespace EcoSort {
    Game *Game::s_instance = nullptr;

    void glfwErrorCallback(int error, const char *description) {
        static Logger logger("GLFW");
        logger.error("Error {}: {}", error, description);
    }

    void Game::run() {
        m_logger.info("Initialising game");

        // If GLFW has an error, it will call this function where I log the error.
        glfwSetErrorCallback(glfwErrorCallback);

        int result;

        // Initialise GLFW so a window can be created.
        result = glfwInit();
        m_logger.assert(result, "Failed to initialize GLFW");
        m_logger.debug("Initialised GLFW");

        // Specify the version of OpenGL that will be used as a window hint. The window hints will apply to all windows
        // created after they are set. This project uses GL 4.1 core, since it is the most recent version supported by
        // macOS.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a new scope so the window will be destroyed once the main loop has finished.
        {
            Window window("EcoSort", 500, 500);
            m_logger.info("Initialised window");

            // Load the OpenGL symbols using glad. This is required, otherwise all the OpenGL function pointers will be
            // null. The symbols are loaded here because glfwGetProcAddress requires a context to be current on the
            // main thread, which is done in the Window initialisation.
            result = gladLoadGL(glfwGetProcAddress);
            m_logger.assert(result, "Failed to initialize GLAD");
            m_logger.debug("OpenGL Version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));

            glClearColor(0, 0, 0, 1);

            // Tell OpenGL to only call the fragment shader for front faces, decided by the winding of their vertices
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);

            // Enable depth testing so that the fragment shader not called for fragments that are behind other
            // fragments.
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            // Tell OpenGL how to blend semi-transparent or fully transparent fragments with what is behind it.
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            int vpWidth, vpHeight;
            window.getFramebufferSize(&vpWidth, &vpHeight);
            glViewport(0, 0, vpWidth, vpHeight);

            Framebuffer gFramebuffer;
            
            Texture gPositions;
            Texture gNormals;
            Texture gAlbedos;
            Texture gDepth;

            gFramebuffer.bind();

            gDepth.bind(0);
            gDepth.setData(nullptr, vpWidth, vpHeight, TextureType::DEPTH);
            gFramebuffer.addDepthAttachment(gDepth);

            gPositions.bind(0);
            gPositions.setData(nullptr, vpWidth, vpHeight, DataType::FLOAT);
            gFramebuffer.addColorAttachment(gPositions, 0);
            gNormals.bind(1);
            gNormals.setData(nullptr, vpWidth, vpHeight, DataType::FLOAT);
            gFramebuffer.addColorAttachment(gNormals, 1);
            gAlbedos.bind(2);
            gAlbedos.setData(nullptr, vpWidth, vpHeight);
            gFramebuffer.addColorAttachment(gAlbedos, 2);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                LOGGER.error("Framebuffer is not complete!");
            }

            Framebuffer lightingFramebuffer;
            lightingFramebuffer.bind();

            Texture lightingTexture;
            
            lightingTexture.bind(0);
            lightingTexture.setData(nullptr, vpWidth, vpHeight);
            lightingFramebuffer.addColorAttachment(lightingTexture, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                LOGGER.error("Framebuffer is not complete!");
            }

            Framebuffer::unbind();

            Shader gBufferVertShader("res/Shaders/gbuffer.vert", ShaderType::VERT);
            Shader gBufferFragShader("res/Shaders/gbuffer.frag", ShaderType::FRAG);
            Shader lightingVertShader("res/Shaders/lighting.vert", ShaderType::VERT);
            Shader lightingFragShader("res/Shaders/lighting.frag", ShaderType::FRAG);
            Shader finalVertShader("res/Shaders/final.vert", ShaderType::VERT);
            Shader finalFragShader("res/Shaders/final.frag", ShaderType::FRAG);

            ShaderProgram gBufferShaderProgram;
            gBufferShaderProgram.attachShader(gBufferVertShader);
            gBufferShaderProgram.attachShader(gBufferFragShader);
            gBufferShaderProgram.link();

            gBufferShaderProgram.use();
            gBufferShaderProgram.setInt("tex", 0);

            ShaderProgram lightingShaderProgram;
            lightingShaderProgram.attachShader(lightingVertShader);
            lightingShaderProgram.attachShader(lightingFragShader);
            lightingShaderProgram.link();

            lightingShaderProgram.use();
            lightingShaderProgram.setInt("gPositions", 0);
            lightingShaderProgram.setInt("gNormals", 1);
            lightingShaderProgram.setInt("gAlbedos", 2);

            ShaderProgram finalShaderProgram;
            finalShaderProgram.attachShader(finalVertShader);
            finalShaderProgram.attachShader(finalFragShader);
            finalShaderProgram.link();

            finalShaderProgram.use();
            finalShaderProgram.setInt("screen", 0);

            auto screenMesh = AssetFetcher::meshFromPath("res/Models/Fullscreen.obj");
            
            Texture texture;
            texture.bind(0);
            texture.setData("res/Textures/img.png");

            glfwSwapInterval(1);

            Object camera = m_scene.createObject();
            auto cameracomp = camera.addComponent<CameraComponent>();
            auto cameraTransform = camera.addComponent<TransformComponent>();

            cameraTransform->position = glm::vec3(0.0f, 0.0f, -10.0f);

            Object object = m_scene.createObject();
            auto objectTransform = object.addComponent<TransformComponent>();
            objectTransform->scale.x = 1;
            
            auto tmpmesh = object.addComponent<Mesh>();
            *tmpmesh = *AssetFetcher::meshFromPath("res/Models/Suzanne.obj");

            double startTime = glfwGetTime();
            int frames = 0;

            int f = 0;

            // While the window is open, i.e. the operating system has not requested for it to be closed.
            while (window.isOpen()) {

                objectTransform->position.x = glm::sin((f * 2 * glm::pi<float>()) / 1000) * 2.0f;
                objectTransform->position.z = glm::cos((f * 2 * glm::pi<float>()) / 1000) * 2.0f;

                f++;

                int framebufferWidth, framebufferHeight;
                window.getFramebufferSize(&framebufferWidth, &framebufferHeight);

                glViewport(0, 0, framebufferWidth, framebufferHeight);
                Framebuffer::unbind();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // GEOMETRY PASS

                gFramebuffer.bind();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                gBufferShaderProgram.use();

                GLenum drawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
                glDrawBuffers(3, drawBuffers);

                auto projection = glm::perspective(
                    glm::radians(cameracomp->fov),
                    static_cast<float>(framebufferWidth) / framebufferHeight, 0.1f, 100.0f
                    );
                auto view = cameraTransform->getTransformation();
                
                gBufferShaderProgram.setMat4("u_projection", glm::value_ptr(projection));
                gBufferShaderProgram.setMat4("u_view", glm::value_ptr(view));

                for (auto& [mesh, transform]: m_scene.findAll<Mesh, TransformComponent>()) {

                    auto model = transform->getTransformation();
                    gBufferShaderProgram.setMat4("u_model", glm::value_ptr(model));

                    auto normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
                    gBufferShaderProgram.setMat3("u_normalMatrix", glm::value_ptr(normalMatrix));

                    texture.bind(0);
                    
                    mesh->draw();
                }

                // LIGHTING PASS

                lightingShaderProgram.use();
                lightingFramebuffer.bind();

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                gPositions.bind(0);
                gNormals.bind(1);
                gAlbedos.bind(2);

                float lightPosition[] = { 0.0f, 0.0f, 0.0f };

                lightingShaderProgram.setFloats("u_lightPosition", lightPosition, 3);
                
                screenMesh->draw();

                // FINAL PASS
                
                Framebuffer::unbind();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                finalShaderProgram.use();

                lightingTexture.bind(0);

                screenMesh->draw();

                // Poll events in GLFW, which will handle OS events and user interfaces, such as the keyboard and mouse.
                glfwPollEvents();

                // Swap the buffers of the window.
                window.update();

                frames++;

                // Reset and display the fps counter if a second has passed
                if (double time = glfwGetTime(); time - startTime > 1.0) {
                    window.setTitle(std::format("EcoSort ({} FPS)", frames).c_str());
                    frames = 0;
                    startTime = time;
                }
            }
        }

        // Clean up GLFW
        glfwTerminate();
    }
}
