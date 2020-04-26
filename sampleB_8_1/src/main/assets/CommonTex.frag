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
    vec4 gColor = texture(sTexture, vTextureCoord)+vec4(0.25,0,0,0)*k;
    fragColor = gColor*vAmbient + gColor*vDiffuse + gColor*vSpecular;
}
    