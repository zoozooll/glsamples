#version 310 es
precision mediump float;

uniform sampler2D sTexture;//纹理内容数据
uniform float utexCoorOffset;//纹理坐标偏移

out vec4 fragColor;//输出到的片元颜色

in vec2 vTextureCoord; //接收从顶点着色器过来的参数
in vec4 vAmbient;	//用于传递给片元着色器的环境光最终强度
in vec4 vDiffuse;	//用于传递给片元着色器的散射光最终强度
in vec4 vSpecular;	//用于传递给片元着色器的镜面光最终强度

void main()                         
{  
   //计算纹理坐标的偏移      
   vec2 texCoorTemp=fract(vTextureCoord+vec2(utexCoorOffset,utexCoorOffset));         
   //给此片元从纹理中采样出颜色值            
   vec4 finalColor=texture(sTexture, texCoorTemp);   
   fragColor =finalColor*vAmbient + finalColor*vDiffuse + finalColor*vSpecular; 
}              