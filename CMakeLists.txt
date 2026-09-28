cmake_minimum_required(VERSION 3.20)

project(OpenGLRoom VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# The repository currently bundles a prebuilt GLFW 3.5.1 Windows library.
# This keeps the project self-contained for the Windows/MSVC environment.
if(NOT WIN32)
    message(FATAL_ERROR
        "This repository currently bundles a Windows GLFW library. "
        "Build it on Windows, or replace the GLFW dependency in CMakeLists.txt."
    )
endif()

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/Libraries")
set(GLFW_INCLUDE_DIR "${THIRD_PARTY_DIR}/include")
set(GLFW_LIBRARY "${THIRD_PARTY_DIR}/lib/glfw3.lib")

if(NOT EXISTS "${GLFW_LIBRARY}")
    message(FATAL_ERROR "Missing GLFW library: ${GLFW_LIBRARY}")
endif()

if(NOT EXISTS "${GLFW_INCLUDE_DIR}/GLFW/glfw3.h")
    message(FATAL_ERROR "Missing GLFW headers in: ${GLFW_INCLUDE_DIR}")
endif()

add_library(glfw STATIC IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${GLFW_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}"
)

add_executable(OpenGLRoom
    src/Main.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/Model.cpp
    src/glad.c
)

target_include_directories(OpenGLRoom PRIVATE
    "${GLFW_INCLUDE_DIR}"
    "${CMAKE_SOURCE_DIR}/src"
)

target_link_libraries(OpenGLRoom PRIVATE
    glfw
    opengl32
    user32
    gdi32
    shell32
)

# Keep runtime assets available next to the executable when they exist.
foreach(asset_dir IN ITEMS shaders textures models)
    if(EXISTS "${CMAKE_SOURCE_DIR}/${asset_dir}")
        add_custom_command(TARGET OpenGLRoom POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/${asset_dir}"
                "$<TARGET_FILE_DIR:OpenGLRoom>/${asset_dir}"
            COMMENT "Copying ${asset_dir} to the output directory"
        )
    endif()
endforeach()

# Helpful warning level for MSVC.
if(MSVC)
    target_compile_options(OpenGLRoom PRIVATE /W4 /permissive-)
endif()
