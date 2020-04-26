#version 300 es
precision mediump float;
in lowp vec4 DestinationColor;
out vec4 fragColor;
void main(void)
{
	fragColor = DestinationColor+vec4(1.0,0.1,0.05,0.0);
}