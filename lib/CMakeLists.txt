cmake_minimum_required(VERSION 3.30)

# Include FetchContent so the FetchContent functions can be used.
include(FetchContent)

# Declare a new library, glad, which will be fetched from Github.
FetchContent_Declare(
        glad_fetch
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG        glad2
        GIT_SHALLOW    TRUE
        SOURCE_SUBDIR  cmake
)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        master
        GIT_SHALLOW    TRUE

        SOURCE_DIR    ${PROJECT_SOURCE_DIR}/lib/glfw
)

# Run the CMakeLists files in the libraries.
FetchContent_MakeAvailable(glfw glad_fetch)

# Declare a new glad target which will be built on project build. It will be linked at build time with 
# the main executable target.
glad_add_library(glad REPRODUCIBLE API gl:core=4.1 LOCATION ${PROJECT_SOURCE_DIR}/lib/glad)