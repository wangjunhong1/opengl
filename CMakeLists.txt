cmake_minimum_required(VERSION 3.15)
project(opengl)
# 头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/include
        ${CMAKE_CURRENT_SOURCE_DIR}/shader_helper)
# 链接库路径
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)
add_subdirectory(00_test_env)
add_subdirectory(01_hello_window)
add_subdirectory(02_hello_triangle)
add_subdirectory(03_shaders)
add_subdirectory(04_textures)
add_subdirectory(05_transformations)