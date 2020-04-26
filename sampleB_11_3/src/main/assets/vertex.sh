#version 300 es
uniform mat4 uVPMatrix;	 						//摄像机透视矩阵
uniform mat4 uMMatrix; 							//基本变换矩阵
uniform float totalNum;							//总的小草棵数
uniform sampler2D sTexture;						//扰动纹理内容数据
uniform sampler2D jbTexture;						//颜色渐变纹理内容数据
in vec3 aPosition;  								//顶点位置
in vec2 aTexCoor;    							//顶点纹理坐标
out vec4 changeTexture;							//改变草地颜色的纹理数据
out vec2 vTextureCoord;  						//用于传递给片元着色器的变量
void xz_Matrix(									//绕轴旋转的方法
		in float posx,							//在x轴上的分量
		in float posy,	 						//在y轴上的分量
		in float posz, 							//在z轴上的分量
		in float xz_cos,							//旋转一定角度的cos值
		in float xz_sin, 							//旋转一定角度的sin值
		out mat4 mmtrix						//输出的旋转矩阵
){
	mmtrix=mat4
	(								//生成旋转矩阵
  		xz_cos+(1.0-xz_cos)*posx*posx , (1.0-xz_cos)*posy*posx - xz_sin*posz ,
			(1.0-xz_cos)*posz*posx + xz_sin*posy,0.0,
 		(1.0-xz_cos)*posx* posy + xz_sin*posz , xz_cos + (1.0-xz_cos)*posy*posy ,
			(1.0-xz_cos)*posz*posy - xz_sin*posx,0.0,
  		(1.0-xz_cos)*posx*posz - xz_sin*posy,(1.0-xz_cos)*posy* posz + xz_sin* (1.0-xz_cos)*
			posx , xz_cos +posz*posz,0.0,
		0.0,0.0,0.0,1.0
	);
}
void main(){
   int colCount=int(sqrt(totalNum));       		//每行多少个
   const float unitSize=0.3; 					//单位平移量
   int col=gl_InstanceID%colCount;				//计算列号
   int row=gl_InstanceID/colCount; 				//计算行号
   float xtex=float(col)/float(colCount); 			//计算出每颗小草在每列中对应的位置
   float ytex=float(row)/float(colCount); 			//计算出每颗小草在每行中对应的位置
   	vec4 noiseVec=texture(sTexture,vec2(xtex,ytex)); //进行扰动纹理采样
   	float dot_product = dot(vec4(noiseVec.rgb,gl_InstanceID), vec4(12.9898,78.233,45.164,94.673));
   	float random=fract(cos(dot_product) * 43758.5453);//获得0.0~1.0的伪随机数
   	float size=random+1.0;						//平移时位置的随机偏移量
   	mat4 pyMatrix=mat4(						//生成平移矩阵
   		1,0,0,0, 
   		0,1,0,0, 
   		0,0,1,0,
   		unitSize*float(col)*size-unitSize,0,unitSize*float(row)*size-unitSize,1//平移的位置
   	);
	mat4 xzMatrix_X;							//生成绕x轴的旋转矩阵
	mat4 xzMatrix_Y;							//生成绕y轴的旋转矩阵
   	float cx_Angle=random*360.0;					//绕y轴--朝向
	float zt_Angle=random*30.0;					//绕x轴--姿态（弯曲程度）
	float xz_Cos=cos(radians(cx_Angle));			//获得绕y轴的cos值
   	float xz_Sin=sqrt(1.0-(xz_Cos*xz_Cos)); 			//获得绕y轴的sin值
   	xz_Matrix(0.0,1.0,0.0,xz_Cos,xz_Sin,xzMatrix_Y);//生成绕y轴旋转矩阵--设置朝向
   	xz_Cos=cos(radians(zt_Angle));				//获得绕x轴的cos值
   	xz_Sin=sqrt(1.0-(xz_Cos*xz_Cos)); 				//获得绕x轴的sin值
	xz_Matrix(1.0,0.0,0.0,xz_Cos,xz_Sin,xzMatrix_X);//生成绕x轴旋转矩阵--设置姿态
   	mat4 MVPMatrix=uVPMatrix*uMMatrix*pyMatrix*xzMatrix_Y*xzMatrix_X;//计算总变换矩阵
   	gl_Position = MVPMatrix * vec4(aPosition,1.0); //根据总变换矩阵计算此次绘制此顶点位置
   	vTextureCoord = aTexCoor;//将接收的纹理坐标传递给片元着色器
   	if(ytex<1.0/totalNum){	//保证颜色的渐变过渡
   		ytex=0.2;			//设置对颜色渐变纹理进行采样时，t值为0.2
   	}
   	vec4 jbTex=texture(jbTexture,vec2(xtex,ytex));//获得颜色渐变的纹理内容数据
	changeTexture=jbTex;	//将采样出的纹理内容数据传递给片元着色器
}