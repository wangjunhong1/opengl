#version 330 core
layout(location=0)in vec3 aPos;// 位置变量的属性位置值为 0

out vec3 ourColor;// 向片段着色器输出一个颜色

void main()
{
    gl_Position=vec4(aPos,1.);
    ourColor=aPos;// 将ourColor设置为我们从顶点数据那里得到的输入颜色
}