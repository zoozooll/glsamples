#version 310 es
precision mediump float;//给出默认的浮点精度
uniform sampler2D sTexture;//纹理内容数据
uniform float utexCoorOffset;//纹理坐标偏移
in vec2 vTextureCoord; //接收从顶点着色器过来的纹理坐标
in vec4 vAmbient;	//接收从顶点着色器过来的环境光最终强度
in vec4 vDiffuse;	//接收从顶点着色器过来的散射光最终强度
in vec4 vSpecular;	//接收从顶点着色器过来的镜面光最终强度
out vec4 fragColor;//输出到的片元颜色
void main()                         
{//主函数           
   //计算纹理坐标的偏移      
   vec2 texCoorTemp=fract(vTextureCoord+vec2(utexCoorOffset,utexCoorOffset));
   //给此片元从纹理中采样出颜色值      
   vec4 finalColor=texture(sTexture, texCoorTemp); 
   //根据光照三个通道的值计算出最终颜色值  
   fragColor =finalColor*vAmbient + finalColor*vDiffuse + finalColor*vSpecular; 
}              