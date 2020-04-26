#version 300 es
uniform mat4 uMVPMatrix; //总变换矩阵
uniform sampler2D jbTexture;//颜色渐变纹理内容数据
in vec3 aPosition;  //顶点位置
in vec2 aTexCoor;    //顶点纹理坐标
out vec4 changeTexture;//改变小草颜色的纹理数据
uniform float jbS;//对应从颜色渐变纹理中采样的S值
uniform float jbT;//对应从颜色渐变纹理中采样的T值
//用于传递给片元着色器的变量
out vec2 vTextureCoord; 
void main()     
{ 
   gl_Position = uMVPMatrix * vec4(aPosition,1); //根据总变换矩阵计算此次绘制此顶点位置  
   vec4 jbTex=texture(jbTexture,vec2(jbS,jbT));//获得颜色渐变的纹理内容数据
   changeTexture=jbTex;//将采样出的纹理内容1数据传递给片元着色器
   vTextureCoord = aTexCoor;//将接收的纹理坐标传递给片元着色器
}                      