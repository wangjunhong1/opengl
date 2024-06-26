//
// Created by wangj on 2024/6/26.
//

#include <common.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
using std::cout;
using std::endl;

void initGlad()
{
    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "初始化GLAD失败." << endl;
        glfwTerminate();
        return;
    }
}

GLFWwindow* initGlfw(int width, int height,const char *  title)
{
    // 初始化GLFW
    glfwInit();
    // 设置OpenGL版本为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 次版本号
    // 设置核心渲染模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr)
    {
        cout << "创建GLFW窗口失败." << endl;
        glfwTerminate();
        return nullptr;
    }
    // 设置窗口上下文
    glfwMakeContextCurrent(window);
    return window;
}

void realseResources(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO)
{
    if (VAO)
        glDeleteVertexArrays(1, VAO);
    if (VBO)
        glDeleteBuffers(1, VBO);
    if (EBO)
        glDeleteBuffers(1, EBO);
}

unsigned int initTexture(const char* path, int colorMode,
                         TexParameter* params, int size)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    for (int i = 0; i < size; i++)
    {
        glTexParameteri(params[i].target, params[i].pname, params[i].param);
    }
    // 加载并生成纹理
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "加载纹理失败" << endl;
    }
    stbi_image_free(data);
    return texture;
}

void initBuffers(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO,
                 float* vertices, int vertexLength,
                 unsigned int* indices, int indicesLength,
                 AttribPointerParams* params, int paramsLength)
{
    // 生成顶点数组对象
    glGenVertexArrays(1, VAO);
    // 生成顶点缓冲对象
    glGenBuffers(1, VBO);
    // 生成元素缓冲对象
    if (EBO)
        glGenBuffers(1, EBO);

    // 绑定VAO（记住先绑定VAO）
    glBindVertexArray(*VAO);
    // 绑定缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    // 绑定元素缓冲对象
    if (EBO)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);

    // 发送数据到缓冲
    glBufferData(GL_ARRAY_BUFFER, vertexLength, vertices, GL_STATIC_DRAW); // 发送顶点数据到顶点缓冲
    if (EBO)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLength, indices, GL_STATIC_DRAW); // 发送索引数据到索引缓冲
    // 设置如何解析顶点
    for (int i = 0; i < paramsLength; i++)
    {
        glVertexAttribPointer(params[i].index, params[i].count, params[i].type,
                              params[i].normalized, params[i].stride, (void*)params[i].offset);
        glEnableVertexAttribArray(params[i].index);
    }

    // 之后可以解绑VBO和VAO，在需要时重新绑定VAO即可（两者之间没有顺序）
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑VBO
    glBindVertexArray(0); // 解绑VAO
    if (EBO)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO的解绑在VAO解绑后
}
