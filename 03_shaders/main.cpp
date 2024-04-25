#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <shader_helper.h>

#define WIDTH 800
#define HEIGHT 600

using std::cout;
using std::endl;
using std::sin;

// 带颜色信息的顶点数组
float vertices[] = {
    // 位置              // 颜色
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
};

unsigned int VAO;
unsigned int VBO;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void initBuffers(unsigned int *VAO, unsigned int *VBO, unsigned int *EBO, float *vertices, int vertexLength)
{
    // 生成顶点数组对象
    glGenVertexArrays(1, VAO);
    // 生成顶点缓冲对象
    glGenBuffers(1, VBO);
    // 生成元素缓冲对象
    // glGenBuffers(1, &EBO);

    // 绑定VAO（记住先绑定VAO）
    glBindVertexArray(*VAO);
    // 绑定缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    // 绑定元素缓冲对象
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 发送数据到缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexLength, vertices, GL_STATIC_DRAW); // 发送顶点数据到顶点缓冲
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 发送索引数据到索引缓冲
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 之后可以解绑VBO和VAO，在需要时重新绑定VAO即可（两者之间没有顺序）
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑VBO
    glBindVertexArray(0);             // 解绑VAO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO的解绑在VAO解绑后
}

int main()
{
    // 初始化GLFW
    glfwInit();
    // 设置OpenGL版本为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 次版本号
    // 设置核心渲染模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "03_shaders", NULL, NULL);
    if (window == NULL)
    {
        cout << "创建GLFW窗口失败." << endl;
        glfwTerminate();
        return -1;
    }
    // 设置窗口上下文
    glfwMakeContextCurrent(window);

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "初始化GLAD失败." << endl;
        glfwTerminate();
        return -1;
    }

    // 设置视口
    glViewport(0, 0, WIDTH, HEIGHT);
    // 注册窗口大小监听器
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 查询OpenGL支持的顶点属性个数
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    cout << "支持的最大顶点属性数是 : " << nrAttributes << endl;

    initBuffers(&VAO, &VBO, NULL, vertices, sizeof(vertices));
    ShaderHelper shaderHelper("D:\\workplace\\opengl\\03_shaders\\vertexShader.glsl",  // 顶点着色器源代码路径
                              "D:\\workplace\\opengl\\03_shaders\\fragmentShader.glsl" // 片段着色器源代码路径
    );

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 1. 处理输入事件
        processInput(window);

        // 2. 渲染指令
        // 清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderHelper.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 3. 检查并调用事件
        glfwPollEvents();
        // 4. 交换颜色缓冲
        glfwSwapBuffers(window);
    }

    // 释放资源
    glfwTerminate();

    return 0;
}