cmake_minimum_required(VERSION 3.17)
project(cgLabs)
if (WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})
endif ()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
    include_directories(libs/glfw/include)
endif ()

include_directories(libs/glfw/include)
set(glm_DIR libs/glm/cmake/glm) # if necessary
set(GLM_INCLUDE_DIR libs/glm/) # if necessary
include_directories(libs/glad/include)
include_directories(libs/assimp/include)
include_directories(libs/glm)
if (APPLE)
add_subdirectory(libs/assimp)
endif ()
find_package(OpenGL REQUIRED)
if (WIN32)
    find_package(GLM REQUIRED)
    find_package(assimp REQUIRED)
    find_package(GLFW3 3.3 REQUIRED)
endif ()
if (APPLE)
    find_package(GLM REQUIRED)
    find_package(GLFW3 REQUIRED)
endif ()

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 20)

add_executable(cgLabs libs/glad/src/glad.c main.cpp application.hpp  window.hpp shader.hpp Buffers/vertex_buffer.hpp Buffers/vertex_array.hpp Buffers/vertex_buffer_layout.hpp buffer.hpp renderer.hpp mesh.hpp Buffers/index_buffer.hpp color_buffer.hpp Buffers/texture_buffer.hpp Buffers/normals_buffer.hpp texture.hpp obj_loader.hpp camera.hpp diffuse_light.hpp lights_manager.hpp)


if (WIN32)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw3 ${GLFW_LIBRARIES} ${GLM_LIBRARIES} assimp)
endif ()
if (APPLE)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw ${GLFW3_LIBRARIES} ${GLM_LIBRARIES} assimp)
endif ()