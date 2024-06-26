//
// Created by wangj on 2024/6/26.
//

#ifndef COMMON_H
#define COMMON_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * 顶点属性绑定参数结构体
 */
struct AttribPointerParams
{
    /**
     * 顶点属性索引 对应顶点着色器中的location
     */
    int index;
    /**
     * 一个这个属性的维度
     */
    int count;
    /**
     * 类型 一般为GL_FLOAT
     */
    int type;
    /**
     * 是否标准化 一般不标准化 GL_FALSE
     */
    int normalized;
    /**
     * 在顶点数组中的步长
     */
    int stride;
    /**
     * 在顶点数组中每个步长中的偏移
     */
    int offset;
};

/**
 * 配置纹理的参数的结构体 例如
 TexParameter texParameter[] = {
        {GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT},
        {GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT},
        {GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST},
        {GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST},
 };
 */
struct TexParameter
{
    /**
     * 需要配置的纹理目标 例如GL_TEXTURE_2D
     */
    int target;
    /**
     * 配置的参数名称 例如 GL_TEXTURE_WRAP_T
     */
    int pname;
    /**
     * 配置的参数值 例如 GL_NEAREST
     */
    int param;
};

/**
 * 初始化glad
 */
void initGlad();

/**
 * 初始化GLFW窗口
 * @param width 窗口的宽度
 * @param height 窗口的长度
 * @param title 窗口的名称
 * @return 窗口对象
 */
GLFWwindow* initGlfw(int width, int height, const char* title);

/**
 * 释放资源 主要是数组
 * @param VAO VAO对象的地址
 * @param VBO VBO对象的地址
 * @param EBO EBO对象的地址
 */
void realseResources(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO);

/**
 * 初始化纹理对象
 * @param path 路径
 * @param colorMode 颜色模式 有GL_RGB和GL_RGBA等
 * @param params 纹理参数 @see TexParameter
 * @param size 纹理参数的数量
 * @return 返回生成的纹理对象
 */
unsigned int initTexture(const char* path, int colorMode,
                         TexParameter* params, int size);

/**
 * 初始化数组
 * @param VAO VAO的地址
 * @param VBO VBO的地址
 * @param EBO EBO的地址
 * @param vertices 顶点数组
 * @param vertexLength 顶点数组的长度
 * @param indices 索引数组
 * @param indicesLength 索引数组的长度
 * @param params 顶点属性的配置参数 @see AttribPointerParams
 * @param paramsLength 顶点属性配置参数的数量
 */
void initBuffers(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO,
                 float* vertices, int vertexLength,
                 unsigned int* indices, int indicesLength,
                 AttribPointerParams* params, int paramsLength);

#endif //COMMON_H
