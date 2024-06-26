#include <common.h>
#include <shader_helper.h>
#include <iostream>
using std::cout;
using std::endl;

#define WIDTH 800
#define HEIGHT 600

using std::cout;
using std::endl;
using std::max;
using std::min;

AttribPointerParams attribPointerParams[] = {
    {0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0},
    {1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float)},
    {2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 6 * sizeof(float)},
};

// 顶点数组
float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // 左上
};
// 元素缓冲对象
unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3, // 第一个三角形
    2, 3, 1 // 第二个三角形
};

// 顶点数组对象
unsigned int VAO;
// 顶点缓冲对象
unsigned int VBO;
// 元素缓冲对象
unsigned int EBO;
// 纹理对象
unsigned int texture0, texture1;

float dot = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

int main()
{
    // 初始化窗口
    GLFWwindow* window = initGlfw(WIDTH,HEIGHT, "04_textures");

    // 初始化GLAD
    initGlad();

    // 设置视口
    glViewport(0, 0, WIDTH, HEIGHT);
    // 注册窗口大小监听器
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 创建着色器
    ShaderHelper shaderHelper("C:\\workplace\\opengl\\04_textures\\vertexShader.glsl",
                              "C:\\workplace\\opengl\\04_textures\\fragmentShader.glsl");

    // 初始化缓冲
    initBuffers(&VAO, &VBO, &EBO,
                vertices, sizeof(vertices),
                indices, sizeof(indices),
                attribPointerParams, sizeof(attribPointerParams));

    TexParameter texParameter[] = {
        {GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT},
        {GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT},
        {GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST},
        {GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST},
    };

    // 初始化纹理
    texture0 = initTexture("C:\\workplace\\opengl\\assets\\container.jpg", GL_RGB, texParameter, sizeof(texParameter));
    texture1 = initTexture("C:\\workplace\\opengl\\assets\\awesomeface.png", GL_RGBA, texParameter,
                           sizeof(texParameter));
    // 设置纹理单元对应的纹理采样器
    shaderHelper.use();
    shaderHelper.setInt("texture1", 1, 0);
    shaderHelper.setInt("texture2", 1, 1);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 1. 处理输入事件
        processInput(window);

        // 2. 渲染指令
        // 清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 绘制
        shaderHelper.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        shaderHelper.setFloat("dot", 1, dot);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 3. 检查并调用事件
        glfwPollEvents();
        // 4. 交换颜色缓冲
        glfwSwapBuffers(window);
    }

    // 释放资源
    realseResources(&VAO, &VBO, &EBO);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        cout << "按下了 ESC 按钮" << endl;
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cout << "按下了 UP 按钮"
            << " , dot = " << dot << endl;
        dot = min(dot + 0.01f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cout << "按下了 DOWN 按钮"
            << " , dot = " << dot << endl;
        dot = max(dot - 0.01f, 0.0f);
    }
}
