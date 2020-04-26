#version 300 es
precision mediump float;
//接收从顶点着色器过来的参数
in vec4 ambient;
in vec4 diffuse;
in vec4 specular;
uniform vec4 aColor;
out vec4 fragColor;//输出到的片元颜色
void main()                         
{    
   //将计算出的颜色给此片元
   fragColor = aColor*ambient+aColor*specular+aColor*diffuse;//给此片元颜色值
}   