#include "CylinderSideInclined.h"
#include "../ShaderUtil.h"
#include "../MatrixState.h"

#include <math.h>

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

#include <stdio.h>
#include <stdlib.h>
#include "../FileUtil.h"

#include <string>
using namespace std;

#define MY_PI 3.141592653

CylinderSideInclined::CylinderSideInclined()
{

}

CylinderSideInclined::CylinderSideInclined(
			int texIdIn,
			float r,//�뾶
			int n,//����������
			btVector3* pos,
			int index
			)
{
	this->n = n;
	vCount = n * 2 * 3;//��������	���ظ�ֵ
	texId = texIdIn;
	this->r = r;
	this->pos = pos;
	this->index = index;
	vertices=new float[vCount*3];//��������
	normals=new float[vCount*3];//����������
	textures=new float[vCount*2];//��������S��T����ֵ����
	refresh();
	initShader();
}

void CylinderSideInclined::initShader()
{

//	LOGI("initShader");

	string strVertex=FileUtil::loadShaderStr("CommonTex.vert");
//	LOGI("str_vertex %s",strVertex.c_str());

	const char* CylinderSideInclinedVertexShader = strVertex.c_str();

	string strFrag=FileUtil::loadShaderStr("CommonTex.frag");
//	LOGI("str_frag %s",strFrag.c_str());

	const char* CylinderSideInclinedFragmentShader = strFrag.c_str();

    mProgram = ShaderUtil::createProgram(CylinderSideInclinedVertexShader, CylinderSideInclinedFragmentShader);
    //��ȡ�����ж���λ����������id
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    //��ȡ�����ж���������������id
    maTexCoorHandle=glGetAttribLocation(mProgram, "aTexCoor");
    //��ȡ�������ܱ任��������id
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    //��ȡλ�á���ת�任��������id
    muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    //��ȡ�����������λ������id
    maCameraHandle=glGetUniformLocation(mProgram, "uCamera");
    //��ȡ���졢��ҹ������������
    mTexHandle=glGetUniformLocation(mProgram, "sTexture");

    //��ȡ���㷨�������Ե�����
    maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
    //��ȡ�����й�Դ��λ�õ�����
    maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");

    mFragK = glGetUniformLocation(mProgram, "k");
    mFlagK = 0.0f;
}

void CylinderSideInclined::refresh()
{
	//�������ݳ�ʼ��
	int count=0;
	int stCount=0;
	int normalCount = 0;
	float angdegSpan=360.0f/n;//ÿ�������ζ���
	for(float angdeg=0;ceil(angdeg)<360;angdeg+=angdegSpan)
	{
		double angrad=angdeg/360.0*(2*MY_PI);//��ǰ����		toRadians(angdeg);
		double angradNext=(angdeg+angdegSpan)/360.0*(2*MY_PI);;//��һ����			toRadians(angdeg+angdegSpan)

		//��Բ��ǰ��---0
		vertices[count++]=(float) (-r*sin(angrad)) + pos[index].x();
		vertices[count++]=pos[index].y();
		vertices[count++]=(float) (-r*cos(angrad)) + pos[index].z();

		normals[normalCount++]=(float) (-r*sin(angrad));
		normals[normalCount++]=0;
		normals[normalCount++]=(float) (-r*cos(angrad));

		textures[stCount++]=(float) (angrad/(2*MY_PI));//st����
		textures[stCount++]=1;
		//��Բ��һ��---3
		vertices[count++]=(float) (-r*sin(angradNext)) + pos[index+1].x();
		vertices[count++]=pos[index+1].y();
		vertices[count++]=(float) (-r*cos(angradNext)) + pos[index+1].z();

		normals[normalCount++]=(float) (-r*sin(angradNext));
		normals[normalCount++]=0;
		normals[normalCount++]=(float) (-r*cos(angradNext));

		textures[stCount++]=(float) (angradNext/(2*MY_PI));//st����
		textures[stCount++]=0;
		//��Բ��ǰ��---2
		vertices[count++]=(float) (-r*sin(angrad)) + pos[index+1].x();
		vertices[count++]=pos[index+1].y();
		vertices[count++]=(float) (-r*cos(angrad)) + pos[index+1].z();

		normals[normalCount++]=(float)(-r*sin(angrad));
		normals[normalCount++]=0;
		normals[normalCount++]=(float)(-r*cos(angrad));

		textures[stCount++]=(float) (angrad/(2*MY_PI));//st����
		textures[stCount++]=0;

		//��Բ��ǰ��---0
//		vertices[count++]=(float) (-r*sin(angrad));
//		vertices[count++]=-h/2;
//		vertices[count++]=(float) (-r*cos(angrad));
		vertices[count++]=(float) (-r*sin(angrad)) + pos[index].x();
		vertices[count++]=pos[index].y();
		vertices[count++]=(float) (-r*cos(angrad)) + pos[index].z();

		normals[normalCount++]=(float)(-r*sin(angrad));
		normals[normalCount++]=0;
		normals[normalCount++]=(float)(-r*cos(angrad));

		textures[stCount++]=(float) (angrad/(2*MY_PI));//st����
		textures[stCount++]=1;
		//��Բ��һ��---1
//		vertices[count++]=(float) (-r*sin(angradNext));
//		vertices[count++]=-h/2;
//		vertices[count++]=(float) (-r*cos(angradNext));
		vertices[count++]=(float) (-r*sin(angradNext)) + pos[index].x();
		vertices[count++]=pos[index].y();
		vertices[count++]=(float) (-r*cos(angradNext)) + pos[index].z();

		normals[normalCount++]=(float)(-r*sin(angradNext));
		normals[normalCount++]=0;
		normals[normalCount++]=(float)(-r*cos(angradNext));

		textures[stCount++]=(float) (angradNext/(2*MY_PI));//st����
		textures[stCount++]=1;
		//��Բ��һ��---3
//		vertices[count++]=(float) (-r*sin(angradNext));
//		vertices[count++]=h/2;
//		vertices[count++]=(float) (-r*cos(angradNext));
		vertices[count++]=(float) (-r*sin(angradNext)) + pos[index+1].x();
		vertices[count++]=pos[index+1].y();
		vertices[count++]=(float) (-r*cos(angradNext)) + pos[index+1].z();

		normals[normalCount++]=(float)(-r*sin(angradNext));
		normals[normalCount++]=0;
		normals[normalCount++]=(float)(-r*cos(angradNext));

		textures[stCount++]=(float) (angradNext/(2*MY_PI));//st����
		textures[stCount++]=0;
	}
	mVertexBuffer = &vertices[0];//���������ݻ��帳ֵ
	mTextureBuffer = &textures[0];//���������ݻ��帳ֵ
	mNormalBuffer = &normals[0];//�����������ݻ��帳ֵ

}

void CylinderSideInclined::drawSelf(int texIdIn)
{
	refresh();

	MatrixState::pushMatrix();//�����ֳ�

    glUseProgram(mProgram);

    glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());
    //��λ�á���ת�任��������ɫ������
    glUniformMatrix4fv(muMMatrixHandle, 1, 0, MatrixState::getMMatrix());
    //�������λ�ô�����ɫ������
    glUniform3fv(maCameraHandle, 1, MatrixState::cameraFB);
	//����Դλ�ô�����ɫ������
	glUniform3fv(maLightLocationHandle, 1, MatrixState::lightPositionFBSun);


    //�����������ж���λ����������id,һ����3�������������ͣ�GL_FALSE��һ����ռ�ֽ������������ݵ��׵�ַ
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3*4, mVertexBuffer);
    //�����������ж���������������id,һ����2�������������ͣ�GL_FALSE��һ����ռ�ֽ���������������׵�ַ
    glVertexAttribPointer(maTexCoorHandle,2,GL_FLOAT,GL_FALSE,2*4,mTextureBuffer);
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

    glEnableVertexAttribArray(maPositionHandle);//�����������ж���λ����������id
    glEnableVertexAttribArray(maTexCoorHandle); //�����������ж���������������id		��������
    glEnableVertexAttribArray(maNormalHandle);//handle ����

    //������
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texIdIn);
    glUniform1i(mTexHandle, 0);
    glUniform1f(mFragK, mFlagK);//mFlagK Ϊ0ʱ��ʾ��������   Ϊ1ʱ��ʾ�к�ɫֵӰ��

    glDrawArrays(GL_TRIANGLES, 0, vCount);//���Ʋ���GL_TRIANGLES��ʽ��0��vCount������		����

    glDisableVertexAttribArray(maPositionHandle);//�����������ж���λ����������id
    glDisableVertexAttribArray(maTexCoorHandle); //�����������ж���������������id		�رգ���
    glDisableVertexAttribArray(maNormalHandle);

	MatrixState::popMatrix();//�ָ��ֳ�
}
