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
	
 	void pointLight (								//ɢ�����ռ���ķ���
  in vec3 normal,								//������
  inout vec4 diffuse,								//ɢ��������
  in vec3 lightLocation,							//��Դλ��
  in vec4 lightDiffuse							//ɢ���ǿ��
){  
  vec3 normalTarget=aPosition+normal;					//����任��ķ�����
  vec3 newNormal=(uMMatrix*vec4(normalTarget,1)).xyz-(uMMatrix*vec4(aPosition,1)).xyz;
  newNormal=normalize(newNormal);					//�Է��������
//����ӱ���㵽��Դλ�õ�����vp
  vec3 vp= normalize(lightLocation-(uMMatrix*vec4(aPosition,1)).xyz);
  vp=normalize(vp);									//���vp
  float nDotViewPosition=max(0.0,dot(newNormal,vp)); 	//��������vp�����ĵ����0�����ֵ
  diffuse=lightDiffuse*nDotViewPosition;			//����ɢ��������ǿ��
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
    