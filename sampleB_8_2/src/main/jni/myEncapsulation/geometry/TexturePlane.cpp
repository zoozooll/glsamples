#include "TexturePlane.h"
#include "../ShaderUtil.h"
#include "../MatrixState.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

#include <stdio.h>
#include <stdlib.h>
#include "../FileUtil.h"

#include <string>
using namespace std;

TexturePlane::TexturePlane(
		float halfWidth, //���
		float halfHeight, //���
		float offset, //λ����
		float UNIT_SIZE//��λ����
		)
{
    initVertexDataRectangle(halfWidth, halfHeight, offset, UNIT_SIZE);
    initShader();
}

void TexturePlane::initVertexDataRectangle(float halfWidth, float halfHeight, float offset, float UNIT_SIZE)
{
	//  3----2
	//  |    |
	//	|	 |
	//  0----1    ˳��Ϊ  0,1,2  0,2,3
    vCount = 6;
    float *vertices=new float[vCount*3];
	int count=0;//���������

	vertices[count++] = -halfWidth*UNIT_SIZE;
	vertices[count++] = offset;
	vertices[count++] = -halfHeight*UNIT_SIZE;

	vertices[count++] = halfWidth*UNIT_SIZE;
	vertices[count++] = offset;
	vertices[count++] = -halfHeight*UNIT_SIZE;


	vertices[count++] = halfWidth*UNIT_SIZE;
	vertices[count++] = offset;
	vertices[count++] = halfHeight*UNIT_SIZE;


	vertices[count++] = -halfWidth*UNIT_SIZE;
	vertices[count++] = offset;
	vertices[count++] = -halfHeight*UNIT_SIZE;


	vertices[count++] = halfWidth*UNIT_SIZE;
	vertices[count++] = offset;
	vertices[count++] = halfHeight*UNIT_SIZE;


	vertices[count++] = -halfWidth*UNIT_SIZE;
	vertices[count++] = offset;
	vertices[count++] = halfHeight*UNIT_SIZE;


	mVertexBuffer = &vertices[0];

    float *normal=new float[vCount*3];
	count=0;//���������
	for(int i = 0; i<vCount; i++)
	{
		normal[count++] = 0;
		normal[count++] = 1;
		normal[count++] = 0;
	}
	mNormalBuffer = &normal[0];

    float *textures=new float[vCount*2];
	count=0;//���������

	textures[count++]=0;
	textures[count++]=0;

	textures[count++]=1;
	textures[count++]=0;

	textures[count++]=1;
	textures[count++]=1;

	textures[count++]=0;
	textures[count++]=0;

	textures[count++]=1;
	textures[count++]=1;

	textures[count++]=0;
	textures[count++]=1;

	mTextureBuffer = &textures[0];
}


void TexturePlane::initShader()
{
//	LOGI("initShader");

	string strVertex=FileUtil::loadShaderStr("Plane.vert");
//	LOGI("str_vertex %s",strVertex.c_str());

	const char* SimpleVertexShader = strVertex.c_str();

	string strFrag=FileUtil::loadShaderStr("Plane.frag");
//	LOGI("str_frag %s",strFrag.c_str());

	const char* SimpleFragmentShader = strFrag.c_str();

	//���ڶ�����ɫ����ƬԪ��ɫ����������
    mProgram = ShaderUtil::createProgram(SimpleVertexShader, SimpleFragmentShader);
    //��ȡ�����ж���λ����������id
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    //��ȡ�����ж��㾭γ����������id
    maTexCoorHandle=glGetAttribLocation(mProgram, "aTexCoor");
    //��ȡ�������ܱ任��������id
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    //��ȡλ�á���ת�任��������id
    muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    //��ȡ�����������λ������id
    maCameraHandle=glGetUniformLocation(mProgram, "uCamera");
    //��ȡ���������
    mTexHandle=glGetUniformLocation(mProgram, "sTexture");

    //��ȡ���㷨�������Ե�����
    maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
    //��ȡ�����й�Դ��λ�õ�����
    maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");

    mFragK = glGetUniformLocation(mProgram, "k");
}

void TexturePlane::drawSelf(const GLint texId)
{
	//�ƶ�ʹ��ĳ��shader����
	glUseProgram(mProgram);
	//�����ձ任������shader����
	glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());
	//��λ�á���ת�任������shader����
	glUniformMatrix4fv(muMMatrixHandle, 1, 0, MatrixState::getMMatrix());
	//�������λ�ô���shader����
	glUniform3fv(maCameraHandle, 1, MatrixState::cameraFB);
	//����Դλ�ô�����ɫ������
	glUniform3fv(maLightLocationHandle, 1, MatrixState::lightPositionFBSun);

	//Ϊ����ָ������λ������
	glVertexAttribPointer
	(
		maPositionHandle,
		3,
		GL_FLOAT,
		GL_FALSE,
		3*4,
		mVertexBuffer
	);
	//Ϊ����ָ������������������
	glVertexAttribPointer
	(
		maTexCoorHandle,
		2,
		GL_FLOAT,
		GL_FALSE,
		2*4,
		mTextureBuffer
	);
	//Ϊ����ָ�����㷨��������
	glVertexAttribPointer
	(
		maNormalHandle,
		3,
		GL_FLOAT,
		GL_FALSE,
		3*4,
		mNormalBuffer
	);

	//������λ����������
	glEnableVertexAttribArray(maPositionHandle);
	glEnableVertexAttribArray(maTexCoorHandle);
	glEnableVertexAttribArray(maNormalHandle);//handle ����

	//������
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);
	glUniform1i(mTexHandle, 0);
	glUniform1f(mFragK, 0);//mFlagK Ϊ0ʱ��ʾ��������   Ϊ1ʱ��ʾ�к�ɫֵӰ��

	//����������
	glDrawArrays(GL_TRIANGLES, 0, vCount);

    glDisableVertexAttribArray(maPositionHandle);
    glDisableVertexAttribArray(maTexCoorHandle);
    glDisableVertexAttribArray(maNormalHandle);
}
