cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

project(00_test_env)

set(PARENT_DIR ../)

file(GLOB SOURCES ${PARENT_DIR}/glad.c main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
 
target_link_libraries(${PROJECT_NAME} glfw3)