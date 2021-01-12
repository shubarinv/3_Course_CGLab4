cmake_minimum_required(VERSION 3.17)
project(cgLabs)
if (WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})
endif ()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(libs/glfw/include)
set(glm_DIR libs/glm/cmake/glm) # if necessary
include_directories(libs/glad/include)
include_directories(libs/assimp/include)
add_subdirectory(libs/assimp)
find_package(OpenGL REQUIRED)
if (WIN32)
    find_package(glm REQUIRED)
    find_package(GLFW3 3.3 REQUIRED)
endif ()
if (APPLE)
    find_package(GLM REQUIRED)
    find_package(GLFW3 REQUIRED)
endif ()

include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 20)

add_executable(cgLabs libs/glad/src/glad.c main.cpp application.hpp  window.hpp shader.hpp vertex_buffer.hpp vertex_array.hpp vertex_buffer_layout.hpp buffer.hpp renderer.hpp mesh.hpp vertex.hpp index_buffer.hpp color_buffer.hpp texture_buffer.hpp normals_buffer.hpp texture.hpp obj_loader.hpp camera.hpp)


if (WIN32)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw3 ${GLFW_LIBRARIES} ${GLM_LIBRARIES} assimp)
endif ()
if (APPLE)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw ${GLFW3_LIBRARIES} ${GLM_LIBRARIES} assimp)
endif ()