cmake_minimum_required(VERSION 3.10)

project(NightDrive)

set(CMAKE_C_STANDARD 99)

find_package(glfw3 REQUIRED)
find_package(glew  REQUIRED)
find_package(Freetype  REQUIRED)

add_executable(NightDrive main.c)

target_link_libraries(NightDrive PRIVATE glfw Freetype::Freetype GLEW::GLEW OpenGL32)