#version 330 core
out vec4 FragColor;

in vec3 posColor;

void main()
{
    FragColor=vec4(posColor,1.);
}