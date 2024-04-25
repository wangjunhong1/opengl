#version 330 core
out vec4 FragColor;

in vec3 ourColor;// 从顶点着色器传递过来的颜色
in vec2 TexCoord;// 从顶点着色器传递过来的纹理坐标

uniform sampler2D ourTexture;// 纹理采样器

void main()
{
    // texture是GLSL内建的函数，用来采样纹理的颜色，它第一个参数是纹理采样器，第二个参数是对应的纹理坐标。
    FragColor=texture(ourTexture,TexCoord);
}