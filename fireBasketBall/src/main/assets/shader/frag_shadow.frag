#version 300 es
precision mediump float;				//给出默认的浮点精度

precision mediump float;
uniform int uisShadowFrag;//是否绘制阴影
uniform int uisLanbanFrag;//是否绘制球在篮板上的阴影
uniform sampler2D usTextureBall;//桌球纹理内容数据

in vec2 vTextureCoord; //接收从顶点着色器过来的参数
in vec4 vambient;
in vec4 vdiffuse;
in vec4 vspecular;
in vec4 vfragLosition;
out vec4 fragColor;
void main()                         
{    
   //将计算出的颜色给此片元
   vec4 finalColor;
   if(uisShadowFrag==1)
   {//绘制阴影，纹理从桌面纹理采样
    finalColor=vec4(0.3,0.3,0.3,0.2);
   	if(uisLanbanFrag==1)
   	{
      if(vfragLosition.x<-9.6||vfragLosition.x>9.6||vfragLosition.y<4.24||vfragLosition.y>10.5)
      {
        finalColor=vec4(0.3,0.3,0.3,0.0);
      }
    }
     
     fragColor = finalColor;//给此片元颜色值
   }
   else
   {
   //绘制球本身，纹理从球纹理采样
     finalColor=texture(usTextureBall, vTextureCoord);
     fragColor = finalColor*vambient+finalColor*vspecular+finalColor*vdiffuse;//给此片元颜色值
   }
}