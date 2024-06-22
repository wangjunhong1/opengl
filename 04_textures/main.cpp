#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader_helper.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define WIDTH 800
#define HEIGHT 600

using std::cout;
using std::endl;
using std::max;
using std::min;

typedef struct AttribPointerParams
{
    int index;
    int count;
    int type;
    int normalized;
    int stride;
    int offset;
} AttribPointerParams;
typedef struct TexParameter
{
    int target;
    int pname;
    int param;
} TexParameter;
AttribPointerParams attribPointerParams[] = {
    {0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0},
    {1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 3 * sizeof(float)},
    {2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 6 * sizeof(float)},
};

// 顶点数组
float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
};
// 元素缓冲对象
unsigned int indices[] = {
    // 注意索引从0开始!
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3, // 第一个三角形
    2, 3, 1  // 第二个三角形
};

// 顶点数组对象
unsigned int VAO;
// 顶点缓冲对象
unsigned int VBO;
// 元素缓冲对象
unsigned int EBO;
// 纹理对象
unsigned int texture, texture2;

float dot = 0.0f;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
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

void initBuffers(unsigned int *VAO, unsigned int *VBO, unsigned int *EBO,
                 float *vertices, int vertexLength,
                 unsigned int *indices, int indicesLength,
                 AttribPointerParams *params, int paramsLength)
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
                              params[i].normalized, params[i].stride, (void *)params[i].offset);
        glEnableVertexAttribArray(params[i].index);
    }

    // 之后可以解绑VBO和VAO，在需要时重新绑定VAO即可（两者之间没有顺序）
    glBindBuffer(GL_ARRAY_BUFFER, 0); // 解绑VBO
    glBindVertexArray(0);             // 解绑VAO
    if (EBO)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO的解绑在VAO解绑后
}

unsigned int initTexture(const char *path, int colorMode,
                         TexParameter *params, int size)
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
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
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

void realseResources(unsigned int *VAO, unsigned int *VBO, unsigned int *EBO)
{
    if (VAO)
        glDeleteVertexArrays(1, VAO);
    if (VBO)
        glDeleteBuffers(1, VBO);
    if (EBO)
        glDeleteBuffers(1, EBO);
}

GLFWwindow *initGlfw()
{
    // 初始化GLFW
    glfwInit();
    // 设置OpenGL版本为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 次版本号
    // 设置核心渲染模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "04_textures", NULL, NULL);
    if (window == NULL)
    {
        cout << "创建GLFW窗口失败." << endl;
        glfwTerminate();
        return NULL;
    }
    // 设置窗口上下文
    glfwMakeContextCurrent(window);
    return window;
}

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

int main()
{
    // 初始化窗口
    GLFWwindow *window = initGlfw();

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
    texture = initTexture("C:\\workplace\\opengl\\04_textures\\container.jpg", GL_RGB, texParameter, sizeof(texParameter));
    texture2 = initTexture("C:\\workplace\\opengl\\04_textures\\awesomeface.png", GL_RGBA, texParameter, sizeof(texParameter));
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
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
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