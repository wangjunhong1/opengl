#version 330 core
out vec4 FragColor;

in vec3 ourColor;// 从顶点着色器传递过来的颜色
in vec2 TexCoord;// 从顶点着色器传递过来的纹理坐标

uniform sampler2D texture1;// 纹理采样器1
uniform sampler2D texture2;// 纹理采样器1

void main()
{
    FragColor=mix(texture(texture1,TexCoord),texture(texture2,TexCoord),.2);
}