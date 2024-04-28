#version 330 core
layout(location=0)in vec3 aPos;// 位置
layout(location=1)in vec2 aTexCoord;// 纹理

out vec2 TexCoord;// 输出纹理

uniform mat4 transform;

void main()
{
    gl_Position=transform * vec4(aPos,1.);
    TexCoord=aTexCoord;// 把纹理坐标传递给片段着色器
}