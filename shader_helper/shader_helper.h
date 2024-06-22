#ifndef SHADER_HELPER_H
#define SHADER_HELPER_H

#include <string>
#include <fstream>
#include <string>

class ShaderHelper
{
private:
    // 程序ID
    unsigned int programId;
    unsigned int buildShader(const char *shaderSource, int shaderType);
    std::string readFile(const char *path);
    unsigned int createProgram(const char *vertexShaderSource, const char *fragmentShaderSource);

public:
    // 构造器读取并构建着色器
    ShaderHelper(const char *vertexPath, const char *fragmentPath);
    // 使用/激活程序
    void use();
    // uniform工具函数
    void setBool(const char *name, int size, ...) const;
    void setInt(const char *name, int size, ...) const;
    void setFloat(const char *name, int size, ...) const;
    unsigned int getProgramId();
};

#endif // SHADER_HELPER_H