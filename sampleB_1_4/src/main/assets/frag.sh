#version 300 es
precision mediump float;
uniform sampler2D sTexture;//廣잿코휭鑒앴
in vec2 vTextureCoord;
out vec4 fragColor;
void main()
{
	fragColor=texture(sTexture, vTextureCoord);
}