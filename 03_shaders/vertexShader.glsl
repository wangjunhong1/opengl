#version 330 core
layout(location=0)in vec3 aPos;// 位置变量的属性位置值为0

uniform float xOffset;

void main()
{
    gl_Position=vec4(aPos.x+xOffset,aPos.yz,1.);// 注意我们如何把一个vec3作为vec4的构造器的参数
}