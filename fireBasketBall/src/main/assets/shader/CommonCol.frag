#version 300 es
in lowp vec4 DestinationColor;
in vec3 outPosition;
out vec4 fragColor;
void main(void)
{	
	float tempy = outPosition.y;
	if(tempy > 4.2)
	{
		fragColor = vec4(1.0,0.0,0.0,1.0);
	}else if(tempy > 4.0)
	{
		fragColor = vec4(0.0,1.0,0.0,1.0);
	}else if(tempy > 3.2)
		{fragColor = vec4(0.0,0.0,1.0,1.0);
	}else
	{
		fragColor = DestinationColor+vec4(1.0,0.1,0.05,0.5);
	}
	
}