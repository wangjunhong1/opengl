# CMake最低版本
cmake_minimum_required(VERSION 3.15)
# C/C++版本
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
# 项目名称
project(05_transformations)
# 源文件
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../libs/glad.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../shader_helper/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/*.cpp
        main.cpp)
# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})
# 链接库
target_link_libraries(${PROJECT_NAME} glfw3)