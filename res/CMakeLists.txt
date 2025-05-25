cmake_minimum_required(VERSION 3.30)

# Get all the files that are descendants of the res directory and store them in ASSET_FILES
file(GLOB_RECURSE ASSET_FILES RELATIVE ${CMAKE_SOURCE_DIR}/res *.* EXCLUDE */CMakeLists.txt)

# Make a command that copies the res directory into the output directory
add_custom_command(
        OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
        DEPENDS ${ASSET_FILES}
        COMMENT "Copying assets to output directory"
        VERBATIM
)

# Make a custom target that depends on the output of the custom command, so it will be run every time the target is
# built, as long a the target is added as a dependency of the target being built
add_custom_target(copy_assets DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res)