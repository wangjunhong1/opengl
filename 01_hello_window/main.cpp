#include <glad/glad.h> // 因为glad中包含了OpenGL需要的头文件，因此先引用glad头文件
#include <GLFW/glfw3.h>
#include <iostream>

using std::cout;
using std::endl;

/**
 * 窗口大小变化的回调函数
 * @param window 上下文中的窗口
 * @param width 变化后的新宽度
 * @param height 变化后的新高度
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // 窗口大小变化后重新设置视口大小
    glViewport(0, 0, width, height);
}

/**
 * 处理输入事件
 * @param window 上下文中的window
 */
void processInput(GLFWwindow *window)
{
    // 如果按了ESC则退出渲染循环
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    // 初始化GLFW
    glfwInit();
    // 设置OpenGL的版本为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 主版本3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 次版本3
    // 设置渲染模式为核心渲染
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow *window = glfwCreateWindow(800, 600, "01_hello_window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 设置窗口上下文
    glfwMakeContextCurrent(window);

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 设置视口
    glViewport(0, 0, 800, 600);
    // 注册窗口大小变化监听函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 1. 处理输入事件
        processInput(window);

        // 2. 渲染指令
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // 设置颜色
        glClear(GL_COLOR_BUFFER_BIT);         // 用指定颜色清空缓冲

        // 3. 检查并调用事件
        glfwPollEvents();
        // 4. 交换颜色缓冲
        glfwSwapBuffers(window);
    }

    // 资源回收
    glfwTerminate();

    return 0;
}