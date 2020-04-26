#version 300 es
precision mediump float;
uniform sampler2D sTexture;
uniform float k;
in vec2 vTextureCoord;
in vec4 vAmbient;
in vec4 vDiffuse;
in vec4 vSpecular;
out vec4 fragColor;
void main()
{
	float s=vTextureCoord.x;
	s=s*24.0-floor(s*24.0);
	float t=vTextureCoord.y;
	t=t*24.0-floor(t*24.0);
	vec2 vTextureCoordTemp=vec2(s,t);
	vec4 gColor =  texture(sTexture, vTextureCoordTemp)+vec4(0.3,0,0,0)*k;;
	
    fragColor = gColor*vAmbient + gColor*vDiffuse + gColor*vSpecular;
}
