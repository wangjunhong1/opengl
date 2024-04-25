#version 330 core
out vec4 FragColor;

in vec3 ourColor;// 从顶点着色器传递过来的颜色
in vec2 TexCoord;// 从顶点着色器传递过来的纹理坐标

uniform sampler2D ourTexture;// 纹理采样器

void main()
{
    // texture是GLSL内建的函数，用来采样纹理的颜色，它第一个参数是纹理采样器，第二个参数是对应的纹理坐标。
    // FragColor=texture(ourTexture,TexCoord);
    // 我们还可以把得到的纹理颜色与顶点颜色混合,我们只需把纹理颜色与顶点颜色在片段着色器中相乘
    FragColor=texture(ourTexture,TexCoord)*vec4(ourColor,1.);
}