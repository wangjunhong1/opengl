#include <common.h>
#include <shader_helper.h>
#include <iostream>
#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WIDTH 800
#define HEIGHT 600

using std::cout;
using std::endl;
using std::max;
using std::min;

AttribPointerParams attribPointerParams[] = {
    {0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0},
    {1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 3 * sizeof(float)},
};

// 顶点数组
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

// 十个正方体的位置
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
};

// 十个正方体的旋转轴
glm::vec3 cubeRotateLine[10];

// 顶点数组对象
unsigned int VAO;
// 顶点缓冲对象
unsigned int VBO;
// 纹理对象
unsigned int texture0, texture1;

float fov = 45.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

// 生成范围在 [min, max] 内的随机浮点数
float random_float(float min, float max);

int main()
{
    // 初始化窗口
    GLFWwindow* window = initGlfw(WIDTH,HEIGHT, "06_coordinate_systems");

    // 初始化GLAD
    initGlad();

    // 设置视口
    glViewport(0, 0, WIDTH, HEIGHT);
    // 注册窗口大小监听器
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 创建着色器
    ShaderHelper shaderHelper("C:\\workplace\\opengl\\06_coordinate_systems\\vertexShader.glsl",
                              "C:\\workplace\\opengl\\06_coordinate_systems\\fragmentShader.glsl");

    // 初始化缓冲
    initBuffers(&VAO, &VBO, NULL,
                vertices, sizeof(vertices),
                NULL, 0,
                attribPointerParams, sizeof(attribPointerParams));

    TexParameter texParameter[] = {
        {GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT},
        {GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT},
        {GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST},
        {GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST},
    };

    // 初始化纹理
    texture0 = initTexture("C:\\workplace\\opengl\\assets\\container.jpg", GL_RGB, texParameter,
                           sizeof(texParameter));
    texture1 = initTexture("C:\\workplace\\opengl\\assets\\awesomeface.png", GL_RGBA, texParameter,
                           sizeof(texParameter));
    // 设置纹理单元对应的纹理采样器
    shaderHelper.use();
    shaderHelper.setInt("texture1", 1, 0);
    shaderHelper.setInt("texture2", 1, 1);

    // 激活深度测试
    glEnable(GL_DEPTH_TEST);

    // 初始化VP矩阵
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // 随机初始化旋转轴
    // 填充数组
    for (int i = 0; i < 10; ++i)
    {
        glm::vec3 random_vec3;
        random_vec3.x = random_float(0.0, 1.0);
        random_vec3.y = random_float(0.0, 1.0);
        random_vec3.z = random_float(0.0, 1.0);
        cubeRotateLine[i] = random_vec3;
    }

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 1. 处理输入事件
        processInput(window);

        // 2. 渲染指令
        // 清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // 绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        // 激活着色器程序
        shaderHelper.use();
        // 传递MVP矩阵
        int modelLocation = glGetUniformLocation(shaderHelper.getProgramId(), "model");
        int viewLocation = glGetUniformLocation(shaderHelper.getProgramId(), "view");
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLocation = glGetUniformLocation(shaderHelper.getProgramId(), "projection");
        projection = glm::perspective(glm::radians(fov),
                                      (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        // 绘制
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, (float)glfwGetTime(), cubeRotateLine[i]);
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // 3. 检查并调用事件
        glfwPollEvents();
        // 4. 交换颜色缓冲
        glfwSwapBuffers(window);
    }

    // 释放资源
    realseResources(&VAO, &VBO, NULL);
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
        cout << "按下了 ESC 键" << endl;
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
    {
        cout << "按下了 UP 键 , fov=" << fov << endl;
        fov += 1.0f;
        if (fov >= 90.f) fov = 90.f;
    }
    if (glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cout << "按下了 DOWN 键 , fov=" << fov << endl;
        fov -= 1.0f;
        if (fov <= 0.0f) fov = 0.0f;
    }
}

// 生成范围在 [min, max] 内的随机浮点数
float random_float(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}
