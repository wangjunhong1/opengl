#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#define WIDTH 800
#define HEIGHT 600

using std::cout;
using std::endl;
using std::sin;

// 顶点着色器源代码
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为0 \n"
                                 "void main()\n"
                                 "{\n"
                                 "    gl_Position = vec4(aPos, 1.0);          // 注意我们如何把一个vec3作为vec4的构造器的参数\n"
                                 "}";

// 片段着色器源代码
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = ourColor;\n"
                                   "}";

// 顶点数组
float vertics[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};

unsigned int VAO;
unsigned int VBO;
unsigned int shaderProgram;

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
    // 设置如何解析顶点
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // 启用顶点属性
    glEnableVertexAttribArray(0);

    // 之后可以解绑VBO和VAO，在需要时重新绑定VAO即可（两者之间没有顺序）
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑VBO
    glBindVertexArray(0);             // 解绑VAO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO的解绑在VAO解绑后
}

unsigned int buildShader(const char *shaderSource, int shaderType)
{
    // 创建顶点着色器
    unsigned int shader = glCreateShader(shaderType);
    // 编译着色器
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    // 查看编译信息
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cout << "着色器编译失败: \n"
             << infoLog << endl;
        return -1;
    }
    cout << "着色器编译成功" << endl;
    return shader;
}

unsigned int createProgram(const char *vertexShaderSource, const char *fragmentShaderSource)
{
    unsigned int vertexShader = buildShader(vertexShaderSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = buildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    // 创建着色器程序
    unsigned int shaderProgram = glCreateProgram();
    // 附加顶点着色器
    glAttachShader(shaderProgram, vertexShader);
    // 附加片段着色器
    glAttachShader(shaderProgram, fragmentShader);
    // 链接顶点着色器和片段着色器
    glLinkProgram(shaderProgram);
    // 查看状态
    int success;
    char infoLog[512];
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
    return shaderProgram;
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

    initBuffers(&VAO, &VBO, NULL, vertics, sizeof(vertics));
    shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 1. 处理输入事件
        processInput(window);

        // 2. 渲染指令
        // 清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        // 从程序中设置uniform变量
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // 随时间生成0-1的浮点数
        int ourColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        if (ourColorLocation != -1)
        {
            // 给Uniform变量赋值
            glUniform4f(ourColorLocation, 0.0f, greenValue, 0.0f, 0.0f);
        }
        else
        {
            cout << "查询不到outColor的uniform变量" << endl;
        }
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