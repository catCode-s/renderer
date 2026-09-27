cmake_minimum_required(VERSION 3.20)

project(Renderer)

find_package(glfw3 REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(Renderer src/main.cpp)

target_link_libraries(Renderer PRIVATE glfw)
target_link_libraries(Renderer PRIVATE glad::glad)