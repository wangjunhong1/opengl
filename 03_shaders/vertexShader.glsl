#version 330 core
layout(location=0)in vec3 aPos;// λ�ñ���������λ��ֵΪ0

void main()
{
    gl_Position=vec4(aPos,1.);// ע��������ΰ�һ��vec3��Ϊvec4�Ĺ������Ĳ���
}