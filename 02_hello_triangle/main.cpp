#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define WIDTH 800
#define HEIGHT 600

using std::cout;
using std::endl;

// 顶点数组
// float vertices[] = {
//     -0.5f, -0.5f, 0.0f,
//     0.5f, -0.5f, 0.0f,
//     0.0f, 0.5f, 0.0f};
float vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};
// 元素缓冲对象
unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};

// 顶点数组对象
unsigned int VAO;
// 顶点缓冲对象
unsigned int VBO;
// 元素缓冲对象
unsigned int EBO;

// 顶点着色器
unsigned int vertexShader;
// 片段着色器
unsigned int fragmentShader;
// 着色器程序
unsigned int shaderProgram;

// 顶点着色器源代码
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

// 片段着色器源代码
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}";

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "02_hello_triangle", NULL, NULL);
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

    // 生成顶点数组对象
    glGenVertexArrays(1, &VAO);
    // 生成顶点缓冲对象
    glGenBuffers(1, &VBO);
    // 生成元素缓冲对象
    glGenBuffers(1, &EBO);

    // 绑定VAO（记住先绑定VAO）
    glBindVertexArray(VAO);
    // 绑定缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 绑定元素缓冲对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 发送数据到缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);       // 发送顶点数据到顶点缓冲
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // 发送索引数据到索引缓冲
    // 设置如何解析顶点
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // 启用顶点属性
    glEnableVertexAttribArray(0);

    // 之后可以解绑VBO和VAO，在需要时重新绑定VAO即可（两者之间没有顺序）
    glBindBuffer(GL_ARRAY_BUFFER, 0);         // 解绑VBO
    glBindVertexArray(0);                     // 解绑VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO的解绑在VAO解绑后

    // 创建顶点着色器
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 编译着色器
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 查看编译信息
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "顶点着色器编译失败: \n"
             << infoLog << endl;
    }
    else
    {
        cout << "顶点着色器编译成功" << endl;
    }

    // 创建片段着色器
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 编译片段着色器
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 查看编译信息
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "片段着色器编译失败: \n"
             << infoLog << endl;
    }
    else
    {
        cout << "片段着色器编译成功" << endl;
    }

    // 创建着色器程序
    shaderProgram = glCreateProgram();
    // 附加顶点着色器
    glAttachShader(shaderProgram, vertexShader);
    // 附加片段着色器
    glAttachShader(shaderProgram, fragmentShader);
    // 链接顶点着色器和片段着色器
    glLinkProgram(shaderProgram);
    // 查看状态
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "着色器链接失败:\n"
             << infoLog << endl;
    }
    else
    {
        cout << "着色器链接成功" << endl;
    }
    // 着色器链接成功之后可以删掉之前的着色器了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 3. 检查并调用事件
        glfwPollEvents();
        // 4. 交换颜色缓冲
        glfwSwapBuffers(window);
    }

    // 释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}