#version 330 core
layout(location=0)in vec3 aPos;// 位置
layout(location=1)in vec3 aColor;// 颜色
layout(location=2)in vec2 aTexCoord;// 纹理

out vec3 ourColor;// 输出颜色
out vec2 TexCoord;// 输出纹理

void main()
{
    gl_Position=vec4(aPos,1.);
    ourColor=aColor;// 把颜色值传递给片段着色器
    TexCoord=aTexCoord;// 把纹理坐标传递给片段着色器
}