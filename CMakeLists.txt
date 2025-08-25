cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)

# Declare a new project
project(EcoSort)

# Add the subdirectory for libraries. This will run the CMakeLists.txt in the lib directory which fetches
# the libraries and makes them available for linking in this file.
add_subdirectory(lib)

# Set the executable output directory for builds and add copy_assets target to this file so res can be copied into the
# same directory as the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(res)

# Create a new target called EcoSort
add_executable(EcoSort
        src/main.cpp
        src/Game.h
        src/Game.cpp
        src/Interface/Logger.tpp
        src/Interface/Logger.h
        src/Interface/Window.cpp
        src/Interface/Window.h
        src/Interface/Interface.h
        src/Interface/Interface.cpp
        src/Scene/Scene.h
        src/Scene/Object.h
        src/Scene/Scene.cpp
        src/Scene/Object.cpp
        src/Graphics/VertexBuffer.cpp
        src/Graphics/VertexBuffer.h
        src/Graphics/VertexArray.h
        src/Graphics/VertexArray.cpp
        src/Graphics/Shader.h
        src/Graphics/Shader.cpp
        src/Graphics/ShaderProgram.h
        src/Graphics/ShaderProgram.cpp
        src/Graphics/IndexBuffer.h
        src/Graphics/IndexBuffer.cpp
        src/AssetFetcher.cpp
        src/AssetFetcher.h
        src/Graphics/Mesh.h
        src/Graphics/Mesh.cpp
        src/Graphics/Texture.h
        src/Graphics/Texture.cpp
        src/Scene/Components.h
        src/Scene/Components.cpp
        src/Graphics/Framebuffer.h
        src/Graphics/Framebuffer.cpp
        src/Interface/Renderer.cpp
        src/Interface/Renderer.h
        src/Graphics/RenderTarget.h
        src/Graphics/RenderTarget.cpp
)

add_compile_options(-std=c++20)

add_dependencies(EcoSort copy_assets)

# Add include directories to the EcoSort target, which will be used to find header files when they
# are included in source or header files. These added directories are PRIVATE, which means any target
# which links this target will not inherit these include directories.
target_include_directories(EcoSort PRIVATE
        lib/glfw/include
        lib/glad/include
        lib/boo/include
        lib/tinyobj
        lib/glm
        lib/stbimage
        lib/qu3e/src
                                        PUBLIC
        src
)

# Link the EcoSort target to the libraries fetched in the lib directory.
target_link_libraries(EcoSort 
        glfw
        glad
        boo
        tinyobjloader
        glm
        stbimage
        qu3e
)

# Add the GLFW_INCLUDE_NONE preprocessor definition to the EcoSort target. This is used to tell
# GLFW not to include OpenGL headers because I want to use the glad symbols.
target_compile_definitions(EcoSort PRIVATE
        GLFW_INCLUDE_NONE
)

# Add a PUBLIC compile definition (definition will be inherited to targets that link this one) that
# if CMake is being built in debug so debug features (such as the debug log severity) can be preprocessed
# out in release builds.
if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(EcoSort PUBLIC
                RG_DEBUG
                RG_DEBUG_SHOW_LIGHTS
        )
endif()



# Configure installation to go into a folder with the resources.
install(TARGETS EcoSort
        RUNTIME DESTINATION bin
)
install(DIRECTORY res/
        DESTINATION bin/res
        FILES_MATCHING PATTERN "*.*"
)