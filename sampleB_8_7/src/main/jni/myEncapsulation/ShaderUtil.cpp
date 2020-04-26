#include "ShaderUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

GLuint ShaderUtil::createProgram(const char* vertexShaderSource,
									  const char* fragmentShaderSource)
{
//	LOGI("createProgram");
	GLuint vertexShader = loadShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	GLuint programHandle = glCreateProgram();
	glAttachShader(programHandle, vertexShader);
	glAttachShader(programHandle, fragmentShader);
	glLinkProgram(programHandle);
	GLint linkSuccess;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
	if (linkSuccess == GL_FALSE) {
		GLchar messages[256];
		glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
		LOGI("==Error== %s",messages);
	}
	return programHandle;
}

GLuint ShaderUtil::loadShader(const char* source, GLenum shaderType)
{
//	LOGI("loadShader");
	GLuint shaderHandle = glCreateShader(shaderType);
	glShaderSource(shaderHandle, 1, &source, 0);
	glCompileShader(shaderHandle);
	GLint compileSuccess;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
	if (compileSuccess == GL_FALSE) {
		GLchar messages[256];
		glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
		LOGI("==Error== %s",messages);
	}
	return shaderHandle;
}


