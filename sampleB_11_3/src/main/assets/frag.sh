#version 300 es
precision mediump float;
uniform sampler2D ssTexture;//纹理内容数据
in vec2 vTextureCoord; //接收从顶点着色器过来的参数
in vec4 changeTexture;
out vec4 fragColor;

void main()
{
   //给此片元从纹理中采样出颜色值
   vec4 finalColor= texture(ssTexture, vTextureCoord);
   fragColor =finalColor*changeTexture;
}