#version 300 es
precision mediump float;
in vec2 vTextureCoord;
uniform sampler2D sTexture;//廣잿코휭鑒앴
out vec4 fragColor;

void main()
{
	fragColor=texture(sTexture, vTextureCoord);
}