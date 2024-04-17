cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

project(02_hello_triangle)

# 设置可执行文件的中文编码集，否则输出中文会乱码
add_compile_options(-fexec-charset=gbk)

set(PARENT_DIR C:/Users/wangj/Documents/opengl)

file(GLOB SOURCES ${PARENT_DIR}/glad.c main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
 
target_link_libraries(${PROJECT_NAME} glfw3)