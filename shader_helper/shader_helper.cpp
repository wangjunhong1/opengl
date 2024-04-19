#include "shader_helper.h"

unsigned int ShaderHelper::buildShader(const char *shaderSource, int shaderType)
{
    const char *shaderName = shaderType == GL_VERTEX_SHADER ? "顶点着色器" : "片段着色器";
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
        std::cout << shaderName << "编译失败: 【" << infoLog << "】" << std::endl;
        return -1;
    }
    else
        std::cout << shaderName << "编译成功" << std::endl;
    return shader;
}

std::vector<std::string> ShaderHelper::readFile(const char *vertexPath, const char *fragmentPath)
{
    std::vector<std::string> sv(2, ""); // 0 vertex , 1 fragment
    FILE *vertexFile = fopen(vertexPath, "rb");
    FILE *fragmentFile = fopen(fragmentPath, "rb");
    char buffer[256];
    if (vertexFile == NULL)
    {
        std::cout << "顶点着色器路径有误" << std::endl;
        return std::vector<std::string>();
    }
    if (fragmentFile == NULL)
    {
        std::cout << "片段着色器路径有误" << std::endl;
        return std::vector<std::string>();
    }
    int size = -1;
    while ((size = fread(buffer, sizeof(char), 256, vertexFile)) > 0)
    {
        if (size < 256)
            buffer[size] = '\0';
        sv[0].append(std::string(buffer, size));
    }
    while ((size = fread(buffer, sizeof(char), 256, fragmentFile)) > 0)
    {
        if (size < 256)
            buffer[size] = '\0';
        sv[1].append(buffer);
    }

    fclose(vertexFile);
    fclose(fragmentFile);

    return sv;
}

unsigned int ShaderHelper::createProgram(const char *vertexShaderSource, const char *fragmentShaderSource)
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
        std::cout << "着色器链接失败: 【" << infoLog << "】" << std::endl;
    }
    else
    {
        std::cout << "着色器链接成功" << std::endl;
    }
    // 着色器链接成功之后可以删掉之前的着色器了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

ShaderHelper::ShaderHelper(const char *vertexPath, const char *fragmentPath)
{
    std::vector<std::string> shaderSources = readFile(vertexPath, fragmentPath);
    programId = createProgram(shaderSources[0].c_str(), shaderSources[1].c_str());
}

void ShaderHelper::use()
{
    glUseProgram(programId);
}

void ShaderHelper::setBool(const char *name, int size, ...) const
{
    va_list args;
    va_start(args, size); // 初始化 args 变量
    int location = glGetUniformLocation(programId, name);
    std::vector<bool> bv;
    for (int i = 0; i < size; i++)
    {
        bv.push_back(va_arg(args, int));
    }
    switch (size)
    {
    case 1:
        glUniform1i(location, bv[0]);
        break;
    case 2:
        glUniform2i(location, bv[0], bv[1]);
        break;
    case 3:
        glUniform3i(location, bv[0], bv[1], bv[2]);
        break;
    case 4:
        glUniform4i(location, bv[0], bv[1], bv[2], bv[3]);
        break;
    default:
        std::cout << "ShaderHelper::setFloat" << std::endl;
    }
    va_end(args); // 清理工作
}

void ShaderHelper::setInt(const char *name, int size, ...) const
{
    va_list args;
    va_start(args, size); // 初始化 args 变量
    int location = glGetUniformLocation(programId, name);
    std::vector<int> iv;
    for (int i = 0; i < size; i++)
    {
        iv.push_back(va_arg(args, int));
    }
    switch (size)
    {
    case 1:
        glUniform1i(location, iv[0]);
        break;
    case 2:
        glUniform2i(location, iv[0], iv[1]);
        break;
    case 3:
        glUniform3i(location, iv[0], iv[1], iv[2]);
        break;
    case 4:
        glUniform4i(location, iv[0], iv[1], iv[2], iv[3]);
        break;
    default:
        std::cout << "ShaderHelper::setFloat" << std::endl;
    }
    va_end(args); // 清理工作
}

void ShaderHelper::setFloat(const char *name, int size, ...) const
{
    va_list args;
    va_start(args, size); // 初始化 args 变量
    int location = glGetUniformLocation(programId, name);
    std::vector<float> fv;
    for (int i = 0; i < size; i++)
    {
        fv.push_back(va_arg(args, double));
    }
    switch (size)
    {
    case 1:
        glUniform1f(location, fv[0]);
        break;
    case 2:
        glUniform2f(location, fv[0], fv[1]);
        break;
    case 3:
        glUniform3f(location, fv[0], fv[1], fv[2]);
        break;
    case 4:
        glUniform4f(location, fv[0], fv[1], fv[2], fv[3]);
        break;
    default:
        std::cout << "ShaderHelper::setFloat参数错误" << std::endl;
    }
    va_end(args); // 清理工作
}

void ShaderHelper::setDouble(const char *name, int size, ...) const
{
    va_list args;
    va_start(args, size); // 初始化 args 变量
    int location = glGetUniformLocation(programId, name);
    std::vector<double> dv;
    for (int i = 0; i < size; i++)
    {
        dv.push_back(va_arg(args, double));
    }
    switch (size)
    {
    case 1:
        glUniform1d(location, dv[0]);
        break;
    case 2:
        glUniform2d(location, dv[0], dv[1]);
        break;
    case 3:
        glUniform3d(location, dv[0], dv[1], dv[2]);
        break;
    case 4:
        glUniform4d(location, dv[0], dv[1], dv[2], dv[3]);
        break;
    default:
        std::cout << "ShaderHelper::setDouble参数错误" << std::endl;
    }
    va_end(args); // 清理工作
}