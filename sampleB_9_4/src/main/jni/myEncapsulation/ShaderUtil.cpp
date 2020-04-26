#include "ShaderUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

GLuint ShaderUtil::createProgram(const char* vertexShaderSource,
									  const char* fragmentShaderSource)
{
//	LOGI("createProgram");
	 //加载顶点着色器
	GLuint vertexShader = loadShader(vertexShaderSource, GL_VERTEX_SHADER);
	//加载片元着色器
	GLuint fragmentShader = loadShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	 //创建程序
	GLuint programHandle = glCreateProgram();
	//向程序中加入顶点着色器
	glAttachShader(programHandle, vertexShader);
	//向程序中加入片元着色器
	glAttachShader(programHandle, fragmentShader);
	//链接程序
	glLinkProgram(programHandle);
	//存放链接成功program数量的值
	GLint linkSuccess;
	//获取program的链接情况
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
	if (linkSuccess == GL_FALSE) {//若链接失败则报错并删除程序
		GLchar messages[256];
		glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
		LOGI("==Error== %s",messages);
	}
	return programHandle;
}

GLuint ShaderUtil::loadShader(const char* source, GLenum shaderType)
{
//	LOGI("loadShader");
	GLuint shaderHandle = glCreateShader(shaderType); //创建一个新shader
	glShaderSource(shaderHandle, 1, &source, 0);//加载shader的源代码
	glCompileShader(shaderHandle);//编译shader
	GLint compileSuccess;//存放编译成功shader数量的值
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);//获取Shader的编译情况
	if (compileSuccess == GL_FALSE) {//若编译失败则显示错误日志并删除此shader
		GLchar messages[256];
		glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
		LOGI("==Error== %s",messages);
	}
	return shaderHandle;
}


