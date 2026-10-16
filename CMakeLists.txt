cmake_minimum_required(VERSION 3.20)

project(Renderer)

find_package(glfw3 REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_executable(Renderer src/main.cpp src/camera.cpp src/shader.cpp src/texture.cpp src/mesh.cpp src/geometry/cube.cpp)

target_include_directories(Renderer PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(Renderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Renderer PRIVATE glfw)
target_link_libraries(Renderer PRIVATE glad::glad)
target_link_libraries(Renderer PRIVATE glm::glm)