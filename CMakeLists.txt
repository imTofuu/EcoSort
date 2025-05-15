cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)

# Declare a new project
project(RecyclingGame)

# Add the subdirectory for libraries. This will run the CMakeLists.txt in the lib directory which fetches
# the libraries and makes them available for linking in this file.
add_subdirectory(lib)

# Create a new target called RecyclingGame
add_executable(RecyclingGame
        src/main.cpp
        src/Game.h
        src/Game.cpp
        src/Interface/Logger.tpp
        src/Interface/Logger.h
        src/Interface/Window.cpp
        src/Interface/Window.h
        src/Interface/Interface.h
        src/Interface/Interface.cpp
)

# Add include directories to the RecyclingGame target, which will be used to find header files when they
# are included in source or header files. These added directories are PRIVATE, which means any target
# which links this target will not inherit these include directories.
target_include_directories(RecyclingGame PRIVATE
        lib/glfw/include
        lib/glad/include
        
                                        PUBLIC
        src
)

# Link the RecyclingGame target to the libraries fetched in the lib directory.
target_link_libraries(RecyclingGame 
        glfw
        glad
)

# Add the GLFW_INCLUDE_NONE preprocessor definition to the RecyclingGame target. This is used to tell
# GLFW not to include OpenGL headers because I want to use the glad symbols.
target_compile_definitions(RecyclingGame PRIVATE
        GLFW_INCLUDE_NONE
)

# Add a PUBLIC compile definition (definition will be inherited to targets that link this one) that
# if CMake is being built in debug so debug features (such as the debug log severity) can be preprocessed
# out in release builds.
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        target_compile_definitions(RecyclingGame PUBLIC
                RG_DEBUG
        )
endif()