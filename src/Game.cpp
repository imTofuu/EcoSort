#include "Game.h"

#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "AssetFetcher.h"
#include <../demo/Clock.h>
#include "Interface/Window.h"
#include "Scene/Components.h"
#include "Scene/Object.h"
#include <dynamics/q3Contact.h>

namespace EcoSort {
    Game *Game::s_instance = nullptr;

    int consumedBoxes = 0;
    int totalBoxes = 3;

    void glfwErrorCallback(int error, const char* description) {
        static Logger logger("GLFW");
        logger.error("Error {}: {}", error, description);
    }

    void Game::BeginContact(const q3ContactConstraint* contact) {
        
        // Validate and convert user data to objects
        
        void* userDataA = contact->bodyA->GetUserData();
        void* userDataB = contact->bodyB->GetUserData();

        if (!userDataA || !userDataB) m_logger.error("Body is missing userData");

        auto objectA = static_cast<Object*>(userDataA);
        auto objectB = static_cast<Object*>(userDataB);

        if (!(objectA && objectA->valid()) || !(objectB && objectB->valid())) return;

        Object* conveyorObject = nullptr;
        Object* collectorObject = nullptr;
        Object* rubbishObject = nullptr;

        if (objectA->getComponent<ConveyorComponent>()) conveyorObject = objectA;
        if (objectB->getComponent<ConveyorComponent>()) conveyorObject = objectB;
        if (objectA->getComponent<CollectorComponent>()) collectorObject = objectA;
        if (objectB->getComponent<CollectorComponent>()) collectorObject = objectB;
        if (objectA->getComponent<RubbishComponent>()) rubbishObject = objectA;
        if (objectB->getComponent<RubbishComponent>()) rubbishObject = objectB;

        // These are collision cases for different interactions.

        // Add rubbish to a list in a conveyor so its velocity can be set every frame.
        if (conveyorObject && rubbishObject) {
            auto conveyorComp = conveyorObject->getComponent<ConveyorComponent>();
            auto rubbishBody = rubbishObject->getComponent<RigidBodyComponent>();
            conveyorComp->touchingRubbish.emplace_back(rubbishBody);
        }

        // Increase the players score if they got the rubbish in the correct collector and move boxes into a unique spot
        // far down so they can be deleted in the game loop.
        if (collectorObject && rubbishObject) {
            auto collectorComp = collectorObject->getComponent<CollectorComponent>();
            auto rubbishComp = rubbishObject->getComponent<RubbishComponent>();
            auto rubbishBody = rubbishObject->getComponent<RigidBodyComponent>();
            if (collectorComp->rubbishType == rubbishComp->type) m_score++;
            consumedBoxes++;
            rubbishBody->body->SetTransform({
                static_cast<float>(-pow(rubbishBody.getEntity() * 200, 3) - 100),
                -100.0f,
                0.0f
            });
        }
    }

    void Game::EndContact(const q3ContactConstraint* contact) {

        // Most of this function is the same as above

        void* userDataA = contact->bodyA->GetUserData();
        void* userDataB = contact->bodyB->GetUserData();

        if (!userDataA || !userDataB) m_logger.error("Body is missing userData");

        auto objectA = static_cast<Object*>(userDataA);
        auto objectB = static_cast<Object*>(userDataB);

        if (!(objectA && objectA->valid()) || !(objectB && objectB->valid())) return;

        Object* conveyorObject = nullptr;
        Object* collectorObject = nullptr;
        Object* rubbishObject = nullptr;

        if (objectA->getComponent<ConveyorComponent>()) conveyorObject = objectA;
        if (objectB->getComponent<ConveyorComponent>()) conveyorObject = objectB;
        if (objectA->getComponent<CollectorComponent>()) collectorObject = objectA;
        if (objectB->getComponent<CollectorComponent>()) collectorObject = objectB;
        if (objectA->getComponent<RubbishComponent>()) rubbishObject = objectA;
        if (objectB->getComponent<RubbishComponent>()) rubbishObject = objectB;

        if (conveyorObject && rubbishObject) {
            auto conveyorComp = conveyorObject->getComponent<ConveyorComponent>();
            auto rubbishBody = rubbishObject->getComponent<RigidBodyComponent>();
            // I can't think of a way to implement an equals operator for a ComponentRef (i the BOO library) so I use
            // std::erase_if with my own lambda predicate that compares the memory addresses of the two components.
            std::erase_if(
                conveyorComp->touchingRubbish,
                [&rubbishBody](BOO::ComponentRef<RigidBodyComponent>& other) -> bool {
                    return other.valid() && rubbishBody.get() == other.get();
                }
            );
        }
        
    }

    Object spawnRubbish(Scene& scene, glm::vec3 position) {
        Object rubbish = scene.createObject();
        auto transform = rubbish.addComponent<TransformComponent>();
        auto mesh = rubbish.addComponent<Mesh>();
        auto rigidBody = rubbish.addComponent<RigidBodyComponent>();
        auto rubbishComp = rubbish.addComponent<RubbishComponent>();
        transform->position = position;
        transform->scale = glm::vec3(5.0f);
        rubbish.setComponent(*AssetFetcher::meshFromPath("res/Models/Cube.obj"));
        mesh->setPrimaryTexture("res/Textures/white.png");
        rigidBody->bodyType = eDynamicBody;
        rigidBody->scale = { 10.0f, 10.0f, 10.0f };

        rubbishComp->type = static_cast<RubbishComponent::RubbishType>(q3RandomInt(0, 2));
        switch (rubbishComp->type) {
            case RubbishComponent::RubbishType::RUBBISH: {
                auto texturePath = std::format("res/Textures/rubbish{}.png", q3RandomInt(0, 2));
                mesh->setPrimaryTexture(texturePath.c_str());
                break;
            }
            case RubbishComponent::RubbishType::RECYCLING: {
                auto texturePath = std::format("res/Textures/recycling{}.png", q3RandomInt(0, 2));
                mesh->setPrimaryTexture(texturePath.c_str());
                break;
            }
            case RubbishComponent::RubbishType::FOOD: {
                auto texturePath = std::format("res/Textures/food{}.png", q3RandomInt(0, 2));
                mesh->setPrimaryTexture(texturePath.c_str());
                break;
            }
            default:
                LOGGER.warn("Invalid rubbish type");
        }
        
        return rubbish;
    }

    void Game::run() {
        m_logger.info("Initialising game");

        // If GLFW has an error, it will call this function where I log the error.
        glfwSetErrorCallback(glfwErrorCallback);

        int result;

        // Initialise GLFW so a window can be created.
        result = glfwInit();
        m_logger.strongAssert(result, "Failed to initialize GLFW");
        m_logger.debug("Initialised GLFW");

        // Specify the version of OpenGL that will be used as a window hint. The window hints will apply to all windows
        // created after they are set. This project uses GL 4.1 core, since it is the most recent version supported by
        // macOS.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a new scope so the window will be destroyed once the main loop has finished.
        {
#ifdef RG_DEBUG
            Window window("EcoSort", 750, 750);
#else
            Window window("EcoSort");
#endif
            m_logger.info("Initialised window");

            // Game extends q3ContactListener so this can be used since that was the most convenient way I could
            // implement collision detection with this library and not making 800 new files. Game::BeginContact will be
            // called when any two bodies collide, and Game::EndContact will be called when they stop
            m_physicsScene.SetContactListener(this);
            // This is more fun
            m_physicsScene.SetEnableFriction(false);

            m_logger.info("Setting up main menu");

            // MAIN MENU -----------------------------------------------------|>

            // This scope prevents accidentally referencing these objects where they are not supposed to be, since
            // objects can be safely deleted without destroying object in the scene.
            {
                // This object code is repeated many times in this file. An object references an entity in an entity
                // component system, by storing the entity ID and the scene. Object in reality is a wrapper object for
                // an entity, only providing some shortcut functions for accessing components and self deletion.
                Object menuCamera = m_menuScene.createObject();
                // By using the data stored in object, a BOO::ComponentRef can be created, which is similar to the
                // object class since it stores an entity ID and a scene. The ref will access components by looking up
                // the entity ID in the scene, through the maps and vectors that are required to find it, instead of
                // just storing a reference to the component since references and pointers to elements in vectors become
                // invalid when the vector is resized.
                auto menuCameraComp = menuCamera.addComponent<CameraComponent>();
                auto menuCameraTransform = menuCamera.addComponent<TransformComponent>();
                // BOO::ComponentRef overloads the -> operator for ease, which just returns a pointer to the current
                // position of the component.
                menuCameraTransform->position = glm::vec3(-100, 0.0f, 0.0f);
                menuCameraTransform->rotation = glm::quatLookAt(-glm::normalize(menuCameraTransform->position), glm::vec3(0.0f, 1.0f, 0.0f));

                Object sun = m_menuScene.createObject();
                auto lightTransform = sun.addComponent<TransformComponent>();
                auto lightComp = sun.addComponent<LightComponent>();
                lightTransform->rotation = glm::quatLookAt(-glm::normalize(glm::vec3(-5.0f, 20.0f, -1.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
                lightComp->type = LightComponent::LightType::DIRECTIONAL;
            }

            // Still main menu, but these references are needed outside the above scope

            Object menuList = m_menuScene.createObject();
            auto menuListTransform = menuList.addComponent<Transform2DComponent>();
            auto menuListComp = menuList.addComponent<GUIFrameComponent>();
            menuListTransform->position = {
                { 0, 0 },
                { 0.5f, 0.5f }
            };
            menuListTransform->size = {
                { 0, 0 },
                { 0.5, 0.5 }
            };

            // This is the worst code maybe i have ever written
            // I don't know why I made GUIs be stored in another component, I think I was going to do something with it,
            // but I can't remember, and it's a bit late now. menuListComp is a vector of shared pointers to pairs of
            // GUIComponent and Transform2DComponent. In other words an owning reference to a key value pair of a
            // GUI element and its transform relative to the GUI frame.
            auto& [ playButton, playButtonTransform ] =
                *menuListComp->guis.emplace_back(
                    std::make_shared<std::pair<GUIComponent, Transform2DComponent>>()
                    );
            playButtonTransform.position = {
                { 0, 0 },
                { 0.0f, -0.15f }
            };
            playButtonTransform.size = {
                { 0, 0 },
                { 1, 0.2 }
            };
            playButton.image = std::make_shared<Texture>();
            playButton.image->setData("res/UI/Play.png");
            
            auto& [ quitButton, quitButtonTransform ] =
                *menuListComp->guis.emplace_back(
                    std::make_shared<std::pair<GUIComponent, Transform2DComponent>>()
                    );
            quitButtonTransform.position = {
                { 0, 0 },
                { 0.0f, 0.15f }
            };
            quitButtonTransform.size = {
                { 0, 0 },
                { 1, 0.2 }
            };
            quitButton.image = std::make_shared<Texture>();
            quitButton.image->setData("res/UI/Quit.png");

            m_logger.info("Setting up game scene");

            // Lots more of initialising scenes, which is in essence the same code as above, but with different self-
            // explanatory components.
            {
                Object gameFlagObject = m_gameScene.createObject();
                auto gameFlag = gameFlagObject.addComponent<IsGameFlagComponent>();
                
                Object gameCamera = m_gameScene.createObject();
                auto gameCameraComp = gameCamera.addComponent<CameraComponent>();
                auto gameCameraTransform = gameCamera.addComponent<TransformComponent>();
                gameCameraTransform->position = glm::vec3(-180.0f, 75.0f, 30.0f);
                gameCameraTransform->rotation = glm::quatLookAt(-glm::normalize(gameCameraTransform->position), glm::vec3(0.0f, 1.0f, 0.0f));

                Object gameSun = m_gameScene.createObject();
                auto gameSunTransform = gameSun.addComponent<TransformComponent>();
                auto gameSunLight = gameSun.addComponent<LightComponent>();
                gameSunTransform->rotation = glm::quatLookAt(-glm::normalize(gameCameraTransform->position + glm::vec3(0.0f, 10.0f, -10.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
                gameSunLight->type = LightComponent::LightType::DIRECTIONAL;
                gameSunLight->colour = glm::vec3(0.7f);

                Object recyclingCollector = m_gameScene.createObject();
                auto recyclingCollectorTransform = recyclingCollector.addComponent<TransformComponent>();
                auto recyclingCollectorRigidBody = recyclingCollector.addComponent<RigidBodyComponent>();
                auto recyclingCollectorComp = recyclingCollector.addComponent<CollectorComponent>();
                auto recyclingCollectorMesh = recyclingCollector.addComponent<Mesh>();
                recyclingCollector.setComponent(*AssetFetcher::meshFromPath("res/Models/Cube.obj"));
                recyclingCollectorMesh->setPrimaryTexture("res/Textures/recycling.jpg");
                recyclingCollectorTransform->position = glm::vec3(-33.0f, 0.0f, (-6 * 11.5f) + (23.0f * 3));
                recyclingCollectorTransform->scale = glm::vec3(12.5f, 3.0f, 11.5f);
                recyclingCollectorRigidBody->bodyType = eStaticBody;
                recyclingCollectorRigidBody->scale = { 25.0f, 100.0f, 23.0f };
                recyclingCollectorComp->rubbishType = RubbishComponent::RubbishType::RECYCLING;

                Object foodCollector = m_gameScene.createObject();
                auto foodCollectorTransform = foodCollector.addComponent<TransformComponent>();
                auto foodCollectorRigidBody = foodCollector.addComponent<RigidBodyComponent>();
                auto foodCollectorComp = foodCollector.addComponent<CollectorComponent>();
                auto foodCollectorMesh = foodCollector.addComponent<Mesh>();
                foodCollector.setComponent(*AssetFetcher::meshFromPath("res/Models/Cube.obj"));
                foodCollectorMesh->setPrimaryTexture("res/Textures/food.png");
                foodCollectorTransform->position = glm::vec3(-33.0f, 0.0f, (-6 * -11.5f) - 23.0f);
                foodCollectorTransform->scale = glm::vec3(12.5f, 3.0f, 11.5f);
                foodCollectorRigidBody->bodyType = eStaticBody;
                foodCollectorRigidBody->scale = { 25.0f, 100.0f, 23.0f };
                foodCollectorComp->rubbishType = RubbishComponent::RubbishType::FOOD;

                Object rubbishCollector = m_gameScene.createObject();
                auto rubbishCollectorTransform = rubbishCollector.addComponent<TransformComponent>();
                auto rubbishCollectorRigidBody = rubbishCollector.addComponent<RigidBodyComponent>();
                auto rubbishCollectorComp = rubbishCollector.addComponent<CollectorComponent>();
                auto rubbishCollectorMesh = rubbishCollector.addComponent<Mesh>();
                rubbishCollector.setComponent(*AssetFetcher::meshFromPath("res/Models/Cube.obj"));
                rubbishCollectorMesh->setPrimaryTexture("res/Textures/rubbish.jpg");
                rubbishCollectorTransform->position = glm::vec3(0.0f, 0.0f, 96.0f);
                rubbishCollectorTransform->scale = glm::vec3(12.5f, 3.0f, 11.5f);
                rubbishCollectorRigidBody->bodyType = eStaticBody;
                rubbishCollectorRigidBody->scale = { 25.0f, 100.0f, 23.0f };
                rubbishCollectorComp->rubbishType = RubbishComponent::RubbishType::RUBBISH;

                for (int i = 0; i < 7; i++) {
                    Object conveyor = m_gameScene.createObject();
                    auto conveyorTransform = conveyor.addComponent<TransformComponent>();
                    auto conveyorMesh = conveyor.addComponent<Mesh>();
                    auto conveyorRigidBody = conveyor.addComponent<RigidBodyComponent>();
                    conveyor.addComponent<ConveyorComponent>();

                    conveyorTransform->position = glm::vec3(0.0f, 0, (-6 * 11.5f) + (23.0f * i));
                    conveyorTransform->rotation = glm::angleAxis(-glm::pi<float>() / 192, glm::vec3(1.0f, 0.0f, 0.0f));

                    auto conveyorMeshPath = std::format("res/Models/{}Conveyor.obj", i == 3 || i == 5 ? "Short" : "");
                    
                    conveyor.setComponent(*AssetFetcher::meshFromPath(conveyorMeshPath.c_str()));
                    conveyorMesh->setPrimaryTexture("res/Textures/white.png");
                    conveyorRigidBody->bodyType = eStaticBody;
                    conveyorRigidBody->scale = { 25.0f, 3.0f, 23.0f };
                    
                    Object light = m_gameScene.createObject();
                    auto lightTransform = light.addComponent<TransformComponent>();
                    auto lightComp = light.addComponent<LightComponent>();
                    lightTransform->position = conveyorTransform->position + glm::vec3(0, 5.0f, 0);
                    lightComp->type = LightComponent::LightType::POINT;
                    lightComp->colour = glm::vec3(1.0f, 0.0f, 0.0f);
                    lightComp->distance = 100.0f;

                    if (i == 3 || i == 5) {
                        Object pusher = m_gameScene.createObject();
                        auto pusherTransform = pusher.addComponent<TransformComponent>();
                        auto pusherMesh = pusher.addComponent<Mesh>();
                        auto pusherRigidBody = pusher.addComponent<RigidBodyComponent>();
                        auto pusherComp = pusher.addComponent<PusherComponent>();
                        pusherTransform->position = glm::vec3(0.0f, 0.0f, (-6 * 11.5f) + 23.0f * i);
                        pusherTransform->rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
                        pusher.setComponent(*AssetFetcher::meshFromPath("res/Models/Pusher.obj"));
                        pusherMesh->setPrimaryTexture("res/Textures/white.png");
                        pusherRigidBody->bodyType = eStaticBody;
                        pusherRigidBody->scale = { 3.15, 100.0f, 23.0f };
                        pusherRigidBody->offset = { -15.5f, 0, 0.0f };
                        pusherComp->activationKey = i == 3 ? Key::Q : Key::E;
                    }
                }
            }

            double startTime = glfwGetTime();
            int frames = 0;
            
            Clock physicsClock;

            // While the window is open, i.e. the operating system has not requested for it to be closed and before the
            // last box has been consumed by a collector.
            while (window.isOpen() && totalBoxes > consumedBoxes) {

                double dt = glfwGetTime() - startTime;
                startTime = glfwGetTime();

                // Poll events in GLFW, which will handle OS events and user interfaces, such as the keyboard and mouse.
                glfwPollEvents();

                // Do gui stuff

                // This chunk of code is what gets the state of each button. It does a simple rectangle intersection
                // test with the position of the mouse, and the position and size of the button.

                Interface& interface = window.getInterface();

                auto mouseX = interface.getMouseX();
                auto mouseY = interface.getMouseY();
                auto mouseEnabled = interface.getMouseButtonEnabledState(MouseButton::LEFT);

                // Several queries are also made in the game loop. Queries are made with a variant of the Scene::find...
                // methods. The variants have different requirements for the components that are present in an entity
                // for it to be returned in the query. The 'All' variant is used exclusively in this project, and it
                // that an entity contains at least all the components specified in order to be returned.
                // BOO::QueryResult is a wrapper type of vector of tuples of the types specified in the query.
                // It defines begin and end methods, which return iterators, so it can be iterated and tuples can be
                // expressed as structured bindings which makes code very readable and clean.
                for (auto& [ guiFrame, frameTransform ] : m_activeScene.findAll<GUIFrameComponent, Transform2DComponent>()) {

                    TransformComponent absoluteFrameTransform = window.getRenderer()->getAbsoluteTransform2D(*frameTransform);

                    for (auto& guiptr : guiFrame->guis) {

                        auto& [ gui, transform ] = *guiptr;

                        TransformComponent absoluteTransform = Renderer::getRelativeTransform2D(transform, absoluteFrameTransform);

                        gui.isHovered = mouseX >= absoluteTransform.position.x - (absoluteTransform.scale.x / 2) &&
                            mouseX <= absoluteTransform.position.x + (absoluteTransform.scale.x / 2) &&
                            mouseY >= absoluteTransform.position.y - (absoluteTransform.scale.y / 2) &&
                            mouseY <= absoluteTransform.position.y + (absoluteTransform.scale.y / 2);

                        gui.isClicked = mouseEnabled && gui.isHovered;
                        
                    }
                }

                // physics

                // This initialises and updates bodies in the physics scene.
                for (auto& [ rigidBody, transform ] : m_activeScene.findAll<RigidBodyComponent, TransformComponent>()) {
                    
                    auto rotAxis = glm::axis(transform->rotation);
                    q3Vec3 q3RotationAxis = { rotAxis.x, rotAxis.y, rotAxis.z };
                    if (!rigidBody->body) {
                        q3BodyDef bodyDef;

                        // This is a memory leak
                        bodyDef.userData = new Object(m_activeScene, rigidBody.getEntity());
                        
                        bodyDef.position = { transform->position.x, transform->position.y, transform->position.z };
                        bodyDef.axis = q3RotationAxis;
                        bodyDef.angle = glm::angle(transform->rotation);
                        bodyDef.bodyType = rigidBody->bodyType;
                        bodyDef.linearVelocity = rigidBody->initialVelocity;
                        bodyDef.angularVelocity = rigidBody->initialAngularVelocity;
                        
                        rigidBody->body = m_physicsScene.CreateBody(bodyDef);

                        q3BoxDef boxDef;
                        q3Transform boxTransform;
                        q3Identity(boxTransform);
                        boxTransform.position = rigidBody->offset;
                        boxTransform.rotation = q3Quaternion(rigidBody->rotationOffsetAxis, rigidBody->rotationOffsetAngle).ToMat3();

                        boxDef.SetRestitution(0);

                        boxDef.Set(boxTransform, rigidBody->scale);
                        rigidBody->body->AddBox(boxDef);
                    }
                    rigidBody->body->SetTransform({
                        transform->position.x,
                        transform->position.y,
                        transform->position.z
                    }, q3RotationAxis, glm::angle(transform->rotation));
                }

                float time = physicsClock.Start();

                // Arbitrary number of iterations that was picked after I closed my eyes and pressed my keyboard. This
                // has no meaning or thought behind it.
                m_physicsScene.SetIterations(8);

                static float accumulator = 0.0f;
                accumulator += time;

                while (accumulator >= 1.0f / 60) {
                    m_physicsScene.Step();
                    accumulator -= 1.0f / 60;
                }

                physicsClock.Stop();

                // This will synchronise all the transforms with the physics bodies.
                for (auto& [ rigidBody, transform ] : m_activeScene.findAll<RigidBodyComponent, TransformComponent>()) {

                    auto& newTransform = rigidBody->body->GetTransform();
                    glm::mat3 rotation = {
                        { newTransform.rotation.Column0()[0], newTransform.rotation.Column1()[0], newTransform.rotation.Column2()[0] },
                        { newTransform.rotation.Column0()[1], newTransform.rotation.Column1()[1], newTransform.rotation.Column2()[1] },
                        { newTransform.rotation.Column0()[2], newTransform.rotation.Column1()[2], newTransform.rotation.Column2()[2] }
                    };

                    transform->position = { newTransform.position.x, newTransform.position.y, newTransform.position.z };
                    transform->rotation = glm::quat_cast(rotation);
                    
                }

                // game updating

                // This moves all the rubbish touching a conveyor in the forward direction of the conveyor
                for (auto& [ conveyor, conveyorTransform ] : m_activeScene.findAll<ConveyorComponent, TransformComponent>()) {
                    
                    auto rotationMatrix = glm::mat3_cast(conveyorTransform->rotation);
                    auto conveyorDirection = rotationMatrix * glm::vec3(0.0f, 0.0f, 1.0f);

                    for (auto& rubbishBody : conveyor->touchingRubbish) {
                        if (!rubbishBody.valid()) continue;
                        rubbishBody->body->SetLinearVelocity({ conveyorDirection.x * 10, 0, conveyorDirection.z * 10 });
                    }
                }

                // This moves the pushers across the conveyors
                for (auto& [ pusher, pusherTransform, pusherRigidBody ] : m_gameScene.findAll<PusherComponent, TransformComponent, RigidBodyComponent>()) {
                    // Intended narrowing conversion since the progress will be 0 when the pusher is not active.
                    if (!pusher->progress && !interface.getKeyEnabledState(pusher->activationKey)) continue;
                    pusher->progress += dt;
                    pusher->progress = glm::clamp(pusher->progress, 0.0f, 2.0f);
                    auto newOffset = 28.0f * (glm::abs(pusher->progress - 1.0f) - 1.0f);
                    pusherTransform->position.x = newOffset;
                    if (pusher->progress >= 2.0f) pusher->progress = 0.0f;
                }

                static int spawnedBoxes = 0;

                static double spawnAccumulator = 0.0;
                static double spawnDelay = 0.1;
                spawnAccumulator += dt;
                // Spawning multiple boxes at the same location makes qu3e a bit angry so I cap to only spawn 1 by using
                // an if instead of a while.
                if (spawnAccumulator >= spawnDelay) {
                    spawnAccumulator -= spawnDelay;
                    auto isGameQuery = m_activeScene.findAll<IsGameFlagComponent>();
                    if (isGameQuery.begin() == isGameQuery.end()) {
                        Object rubbish = spawnRubbish(m_menuScene, glm::vec3(0, 70, q3RandomFloat(-60, 60)));
                        rubbish.getComponent<RigidBodyComponent>()->initialAngularVelocity = {
                            q3RandomFloat(-1.0f, 1.0f),
                            q3RandomFloat(-1.0f, 1.0f),
                            q3RandomFloat(-1.0f, 1.0f)
                        };
                    } else if (totalBoxes > spawnedBoxes) {
                        // Change the spawn delay once the game scene is active since this piece of code is also used
                        // for main menu decoration.
                        spawnDelay = 5.0;
                        spawnRubbish(m_activeScene, glm::vec3(0.0f, 10.0f, -5 * 11.5f));
                        spawnedBoxes++;
                    }
                }

                // Delete boxes that have fallen too far, since allocating hundreds of textures with tens of thousands
                // of pixels uses a considerable amount of memory. If I cached textures that wouldn't be a problem but
                // that's time I have to spend.
                for (auto& [ _, transform ] : m_activeScene.findAll<RubbishComponent, TransformComponent>()) {
                    if (transform->position.y < -200.0f) {
                        Object object(m_activeScene, transform.getEntity());
                        m_activeScene.removeObject(object);
                    }
                }

                // qu3e did not appreciate having multiple physics scenes, and since it is a small library it was hard
                // to find out why. This was the easiest way I found to fix the problem.
                if (playButton.isClicked) {
                    m_physicsScene.RemoveAllBodies();

                    for (auto& [ rigidBody, transform ] : m_activeScene.findAll<RigidBodyComponent, TransformComponent>()) {
                        rigidBody->body = nullptr;
                    }
                    
                    m_activeScene = m_gameScene;
                }
                if (quitButton.isClicked) break;

                // Swap the buffers of the window.
                window.update();

                frames++;

                // Reset and display the fps counter if a second has passed
                static double frameAccumulator = 0.0f;
                frameAccumulator += dt;
                if (frameAccumulator >= 1.0) {
                    window.setTitle(std::format("EcoSort ({} FPS)", frames).c_str());
                    frames = 0;
                    frameAccumulator = 0;
                }
            }
        }

        m_logger.info("\n\n\nGame score: {} / {}", m_score, totalBoxes);

        // Clean up GLFW
        glfwTerminate();
    }
}
