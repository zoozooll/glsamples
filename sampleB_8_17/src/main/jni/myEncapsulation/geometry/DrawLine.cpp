#include "DrawLine.h"
#include "..\ShaderUtil.h"
#include "..\MatrixState.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

#include <stdio.h>
#include <stdlib.h>
#include "..\FileUtil.h"

#include <string>
using namespace std;

DrawLine::DrawLine()
{
	this->vCount = 2;//��������
	this->colors = new float[vCount * 4];
	this->vertices = new float[vCount * 3];

	for(int i = 0; i < vCount; i++)
	{
		colors[i * 4] = 1;
		colors[i * 4 + 1] = 1;
		colors[i * 4 + 2] = 0;
		colors[i * 4 + 3] = 1;
	}
	mColorBuffer = &colors[0];

	initShader();

}

DrawLine::DrawLine(
		float *vertices//������������  ����Ӧ��Ϊ6
		)
{
	this->vCount = 2;//��������
	this->colors = new float[vCount * 4];

	for(int i = 0; i < vCount; i++)
	{
		colors[i * 4] = 0;
		colors[i * 4 + 1] = 1;
		colors[i * 4 + 2] = 0;
		colors[i * 4 + 3] = 1;
	}

	mVertexBuffer = &vertices[0];
	mColorBuffer = &colors[0];

	initShader();
}
DrawLine::DrawLine(
		btVector3 vec0,//��һ������
		btVector3 vec1//�ڶ�������
		)
{
	this->vCount = 2;//��������
	this->colors = new float[vCount * 4];
	this->vertices = new float[vCount * 3];

	for(int i = 0; i < vCount; i++)
	{
		colors[i * 4] = 0;
		colors[i * 4 + 1] = 1;
		colors[i * 4 + 2] = 0;
		colors[i * 4 + 3] = 1;
	}

	vertices[0] = vec0.x();
	vertices[1] = vec0.y();
	vertices[2] = vec0.z();

	vertices[3] = vec1.x();
	vertices[4] = vec1.y();
	vertices[5] = vec1.z();



	mVertexBuffer = &vertices[0];
	mColorBuffer = &colors[0];

	initShader();
}



void DrawLine::initShader()
{
		string strVertex=FileUtil::loadShaderStr("CommonCol.vert");

		const char* ClothVertexShader = strVertex.c_str();

		string strFrag=FileUtil::loadShaderStr("CommonCol.frag");
//		LOGI("str_frag %s",strFrag.c_str());

		const char* ClothFragmentShader = strFrag.c_str();

	    mProgram = ShaderUtil::createProgram(ClothVertexShader, ClothFragmentShader);
	    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
	    maColorHandle = glGetAttribLocation(mProgram, "sColor");
	    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
}

void DrawLine::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

    glUseProgram(mProgram);

    glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());

    glEnableVertexAttribArray(maPositionHandle);
    glEnableVertexAttribArray(maColorHandle);

    glVertexAttribPointer(maPositionHandle, 3, GL_FLOAT, GL_FALSE, 3*4, mVertexBuffer);
    glVertexAttribPointer(maColorHandle, 4, GL_FLOAT, GL_FALSE, 4*4, mColorBuffer);
    glDrawArrays(GL_LINES, 0, 2);//������������vCountLine
    glLineWidth(4.0f);

    glDisableVertexAttribArray(maPositionHandle);
    glDisableVertexAttribArray(maColorHandle);

	MatrixState::popMatrix();//�ָ��ֳ�
}

void DrawLine::initVertex(
		btVector3 vec0,//��һ������
		btVector3 vec1//�ڶ�������
		)
{
	vertices[0] = vec0.x();
	vertices[1] = vec0.y();
	vertices[2] = vec0.z();

	vertices[3] = vec1.x();
	vertices[4] = vec1.y();
	vertices[5] = vec1.z();

	mVertexBuffer = &vertices[0];
}

void DrawLine::drawSelf(
		btVector3 vec0,//��һ������
		btVector3 vec1//�ڶ�������
		)
{
	initVertex(vec0, vec1);
	drawSelf();
}
