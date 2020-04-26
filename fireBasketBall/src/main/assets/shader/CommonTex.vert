  #version 300 es
	uniform mat4 uMVPMatrix;
	uniform mat4 uMMatrix;
	in vec3 aPosition;
	in vec2 aTexCoor;
	in vec3 aNormal;
	out vec2 vTextureCoord;
	void main()
	{
		gl_Position = uMVPMatrix * vec4(aPosition,1);
		vTextureCoord = aTexCoor;
	}
    