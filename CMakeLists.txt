cmake_minimum_required(VERSION 3.15)
project(opengl)
# 头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/include
        ${CMAKE_CURRENT_SOURCE_DIR}/shader_helper)
# 链接库路径
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

# 获取当前目录下所有子目录的列表
file(GLOB SUBDIR_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
# 遍历所有子目录
foreach(SUBDIR ${SUBDIR_LIST})
    # 如果是目录、不是以点开头（排除隐藏目录）、并且存在 CMakeLists.txt 文件，则添加到项目中
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}
            AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/CMakeLists.txt)
        message("the cmake directory is: ${SUBDIR}")
        add_subdirectory(${SUBDIR})
    endif()
endforeach()