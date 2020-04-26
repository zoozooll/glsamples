#version 300 es
precision mediump float;				//给出默认的浮点精度
uniform vec4 aColor;
in vec4 ambient;  				//从顶点着色器传递过来的环境光最终强度
in vec4 diffuse;					//从顶点着色器传递过来的散射光最终强度
in vec4 specular;				//从顶点着色器传递过来的镜面光最终强度
out vec4 fragColor;//输出到的片元颜色
void main() {
	vec4 finalColor=aColor;		//物体本身的颜色
	//综合三个通道光的最终强度及片元的颜色计算出最终片元的颜色并传递给管线
	fragColor = finalColor*ambient+finalColor*specular+finalColor*diffuse;
}     