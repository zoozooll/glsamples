  #version 300 es
	uniform mat4 uMVPMatrix;
	uniform mat4 uMMatrix;
	uniform vec3 uLightLocation;
	in vec3 aPosition;
	in vec2 aTexCoor;
	in vec3 aNormal;
	out vec2 vTextureCoord;
	out vec4 vAmbient;	
	out vec4 vDiffuse;
	
 	void pointLight (								//散射光光照计算的方法
  in vec3 normal,								//法向量
  inout vec4 diffuse,								//散射光计算结果
  in vec3 lightLocation,							//光源位置
  in vec4 lightDiffuse							//散射光强度
){  
  vec3 normalTarget=aPosition+normal;					//计算变换后的法向量
  vec3 newNormal=(uMMatrix*vec4(normalTarget,1)).xyz-(uMMatrix*vec4(aPosition,1)).xyz;
  newNormal=normalize(newNormal);					//对法向量规格化
//计算从表面点到光源位置的向量vp
  vec3 vp= normalize(lightLocation-(uMMatrix*vec4(aPosition,1)).xyz);
  vp=normalize(vp);									//规格化vp
  float nDotViewPosition=max(0.0,dot(newNormal,vp)); 	//求法向量与vp向量的点积与0的最大值
  diffuse=lightDiffuse*nDotViewPosition;			//计算散射光的最终强度
}
	void main()
	{
		gl_Position = uMVPMatrix * vec4(aPosition,1);
		vTextureCoord = aTexCoor;
		
		vec4 diffuseTemp = vec4(0.0,0.0,0.0,0.0);
		
		pointLight(normalize(aNormal),diffuseTemp,uLightLocation,vec4(0.8,0.8,0.8,1));
		vDiffuse=diffuseTemp;
		vAmbient = vec4(0.6,0.6,0.6,1.0);
	}
    