#version 300 es
precision mediump float;
uniform sampler3D sTexture;//3D纹理内容数据
//接收从顶点着色器过来的参数
in vec4 ambient;//接收从顶点着色器过来的环境光最终强度
in vec4 diffuse;//接收从顶点着色器过来的散射光最终强度
in vec4 specular;//接收从顶点着色器过来的镜面光最终强度
in vec3 vPosition;//接收从顶点着色器过来的位置坐标  
out vec4 fragColor;//最终输出的片元颜色
void main()                         
{  
   //浅色木纹颜色
   const vec4 lightWood=vec4(0.6,0.3,0.1,1.0);
   //深色木纹颜色
   const vec4 darkWood=vec4(0.4,0.2,0.07,1.0);

   //根据片元的位置折算出3D纹理坐标--3D噪声纹理的索引
   vec3 texCoor=vec3(((vPosition.x/0.52)+1.0)/2.0,vPosition.y/0.4,((vPosition.z/0.52)+1.0)/2.0);
   //进行3D纹理采样
   vec4 noiseVec=texture(sTexture,texCoor); 
   
   //计算受3D柏林噪声纹理采样值影响的位置
   vec3 location=vPosition+noiseVec.rgb*0.05;
   //计算离平面中心点的距离
   float dis=distance(location.xz,vec2(0,0)); 
   //年轮的频率 
   dis *=2.0;//控制圆环的多少
   //计算两种木纹颜色的混合因子
   float r=fract(dis+noiseVec.r+noiseVec.g*0.5+noiseVec.b*0.25+noiseVec.a*0.125)*2.0;
   if(r>1.0)
   {
      r=2.0-r;
   }
   //进行两种木纹颜色的混合
   vec4 color=mix(lightWood,darkWood,r);
   
   //再次计算调整因子
   r=fract((location.y+location.x+location.z)*25.0+0.5);
   noiseVec.a*=r;//修改噪声值
   //根据调整因子调整颜色
   if(r<0.5)
   {
   	  color+=lightWood*1.0*noiseVec.a; 
   }
   else
   {
      color-=lightWood*0.02*noiseVec.a;  
   }
   //给此片元颜色值
   fragColor =color*ambient+color*specular+color*diffuse;
}   