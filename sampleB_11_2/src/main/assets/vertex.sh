#version 300 es
uniform mat4 uVPMatrix; //摄像机透视矩阵
uniform mat4 uMMatrix; //基本变换矩阵
in vec3 aPosition;  //顶点位置
in vec2 aTexCoor;    //顶点纹理坐标
out vec2 vTextureCoord;  //用于传递给片元着色器的变量
void main()     
{              
   //每行多少个
   const int colCount=3;      
   //单位平移量
   const float unitSize=4.0;
   //计算列号
   int col=gl_InstanceID%colCount;
   //计算行号
   int row=gl_InstanceID/colCount;
   //生成平移矩阵
   mat4 pyMatrix=mat4
   (
   		1,0,0,0, 
   		0,1,0,0, 
   		0,0,1,0, 
   		unitSize*float(col)-unitSize,unitSize*float(row)-unitSize,0,1
   );
   //计算总变换矩阵
   mat4 MVPMatrix=uVPMatrix*uMMatrix*pyMatrix;
   //根据总变换矩阵计算此次绘制此顶点位置
   gl_Position = MVPMatrix * vec4(aPosition,1.0); 
   vTextureCoord = aTexCoor;//将接收的纹理坐标传递给片元着色器
}