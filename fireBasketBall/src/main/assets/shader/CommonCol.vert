#version 300 es
in vec3 aPositionOrigi;
in vec3 aPosition;
in vec4 sColor;
out vec4 DestinationColor;
out vec3 outPosition;
uniform mat4 uMVPMatrix;
void main(void)
{
   DestinationColor = sColor;
   gl_Position = uMVPMatrix * vec4(aPosition,1);
   outPosition = aPositionOrigi;
}