#include "ShaderUtil.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

GLuint ShaderUtil::createProgram(const char* vertexShaderSource,
									  const char* fragmentShaderSource)
{
//	LOGI("createProgram");
	 //���ض�����ɫ��
	GLuint vertexShader = loadShader(vertexShaderSource, GL_VERTEX_SHADER);
	//����ƬԪ��ɫ��
	GLuint fragmentShader = loadShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	 //��������
	GLuint programHandle = glCreateProgram();
	//������м��붥����ɫ��
	glAttachShader(programHandle, vertexShader);
	//������м���ƬԪ��ɫ��
	glAttachShader(programHandle, fragmentShader);
	//���ӳ���
	glLinkProgram(programHandle);
	//������ӳɹ�program������ֵ
	GLint linkSuccess;
	//��ȡprogram���������
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
	if (linkSuccess == GL_FALSE) {//������ʧ���򱨴�ɾ������
		GLchar messages[256];
		glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
		LOGI("==Error== %s",messages);
	}
	return programHandle;
}

GLuint ShaderUtil::loadShader(const char* source, GLenum shaderType)
{
//	LOGI("loadShader");
	GLuint shaderHandle = glCreateShader(shaderType); //����һ����shader
	glShaderSource(shaderHandle, 1, &source, 0);//����shader��Դ����
	glCompileShader(shaderHandle);//����shader
	GLint compileSuccess;//��ű���ɹ�shader������ֵ
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);//��ȡShader�ı������
	if (compileSuccess == GL_FALSE) {//������ʧ������ʾ������־��ɾ����shader
		GLchar messages[256];
		glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
		LOGI("==Error== %s",messages);
	}
	return shaderHandle;
}


