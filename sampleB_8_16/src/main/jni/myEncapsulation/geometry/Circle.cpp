#include "Circle.h"
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

Circle::Circle(
			int texIdIn,
			float m_r,//�뾶
			int m_n,//Բ���и�ķ���
			float h//Բ�ĸ߶�
			)
{
	float angdegSpan=360.0f/m_n;//ÿ�������ζ���
	m_vCount = m_n * 3;//��������	���ظ�ֵ
	m_texId = texIdIn;
	this->needRefresh = false;
	float* m_vertices=new float[m_vCount*3];//��������
	float* m_normal=new float[m_vCount*3];//��������
	float* m_textures=new float[m_vCount*2];//��������S��T����ֵ����
	//�������ݳ�ʼ��
	int count=0;
	int stCount=0;
	int normalCount =0;
	for(float angdeg=0;ceil(angdeg)<360;angdeg+=angdegSpan)
	{
		double angrad=angdeg/360.0*(2*MY_PI);//��ǰ����		toRadians(angdeg);
		double angradNext=(angdeg+angdegSpan)/360.0*(2*MY_PI);;//��һ����			toRadians(angdeg+angdegSpan)
		//���ĵ�
		m_vertices[count++]=0;//��������
		m_vertices[count++]=h;
		m_vertices[count++]=0;

		m_normal[normalCount++] = 0;//����������
		m_normal[normalCount++] = 1;
		m_normal[normalCount++] = 0;

		m_textures[stCount++]=0.5f;//st����
		m_textures[stCount++]=0.5f;
		//��ǰ��
		m_vertices[count++]=(float) (-m_r*sin(angrad));//��������
		m_vertices[count++]=h;
		m_vertices[count++]=(float) (m_r*cos(angrad));

		m_normal[normalCount++] = 0;//����������
		m_normal[normalCount++] = 1;
		m_normal[normalCount++] = 0;

		m_textures[stCount++]=(float) (0.5f-0.5f*sin(angrad));//st����
		m_textures[stCount++]=(float) (0.5f-0.5f*cos(angrad));
		//��һ��
		m_vertices[count++]=(float) (-m_r*sin(angradNext));//��������
		m_vertices[count++]=h;
		m_vertices[count++]=(float) (m_r*cos(angradNext));

		m_normal[normalCount++] = 0;//����������
		m_normal[normalCount++] = 1;
		m_normal[normalCount++] = 0;

		m_textures[stCount++]=(float) (0.5f-0.5f*sin(angradNext));//st����
		m_textures[stCount++]=(float) (0.5f-0.5f*cos(angradNext));
	}
	mVertexBuffer = &m_vertices[0];//���������ݻ��帳ֵ
	mTextureBuffer = &m_textures[0];//���������ݻ��帳ֵ
	mNormalBuffer = &m_normal[0];//�����������ݻ��帳ֵ

	initShader();
}

Circle::Circle(
			int texIdIn,
			float m_r,//�뾶
			int m_n,//Բ���и�ķ���
			btVector3* m_pos,
			int m_index
			)
{

	m_vCount = m_n * 3;//��������	���ظ�ֵ
	m_texId = texIdIn;
	this->needRefresh = true;
	this->m_pos = m_pos;
	this->m_index = m_index;
	this->m_r = m_r;
	this->m_n = m_n;
	m_vertices=new float[m_vCount*3];//��������
	m_normal=new float[m_vCount*3];//��������
	m_textures=new float[m_vCount*2];//��������S��T����ֵ����
	//�������ݳ�ʼ��
	int count=0;
	int stCount=0;
	int normalCount =0;
	float angdegSpan=360.0f/m_n;//ÿ�������ζ���
	for(float angdeg=0;ceil(angdeg)<360;angdeg+=angdegSpan)
	{
		double angrad=angdeg/360.0*(2*MY_PI);//��ǰ����		toRadians(angdeg);
		double angradNext=(angdeg+angdegSpan)/360.0*(2*MY_PI);;//��һ����			toRadians(angdeg+angdegSpan)
		//���ĵ�
		m_vertices[count++]=0 + m_pos[m_index].x();//��������
		m_vertices[count++]=m_pos[m_index].y();
		m_vertices[count++]=0 + m_pos[m_index].z();

		m_normal[normalCount++] = 0;//����������
		m_normal[normalCount++] = 1;
		m_normal[normalCount++] = 0;

		m_textures[stCount++]=0.5f;//st����
		m_textures[stCount++]=0.5f;
		//��ǰ��
		m_vertices[count++]=(float) (-m_r*sin(angrad)) + m_pos[m_index].x();//��������
		m_vertices[count++]=m_pos[m_index].y();
		m_vertices[count++]=(float) (m_r*cos(angrad)) + m_pos[m_index].z();

		m_normal[normalCount++] = 0;//����������
		m_normal[normalCount++] = 1;
		m_normal[normalCount++] = 0;

		m_textures[stCount++]=(float) (0.5f-0.5f*sin(angrad));//st����
		m_textures[stCount++]=(float) (0.5f-0.5f*cos(angrad));
		//��һ��
		m_vertices[count++]=(float) (-m_r*sin(angradNext)) + m_pos[m_index].x();//��������
		m_vertices[count++]=m_pos[m_index].y();
		m_vertices[count++]=(float) (m_r*cos(angradNext)) + m_pos[m_index].z();

		m_normal[normalCount++] = 0;//����������
		m_normal[normalCount++] = 1;
		m_normal[normalCount++] = 0;

		m_textures[stCount++]=(float) (0.5f-0.5f*sin(angradNext));//st����
		m_textures[stCount++]=(float) (0.5f-0.5f*cos(angradNext));
	}
	mVertexBuffer = &m_vertices[0];//���������ݻ��帳ֵ
	mTextureBuffer = &m_textures[0];//���������ݻ��帳ֵ
	mNormalBuffer = &m_normal[0];//�����������ݻ��帳ֵ

	initShader();
}

void Circle::initShader()
{
//	LOGI("initShader");

	string strVertex=FileUtil::loadShaderStr("CommonTex.vert");
//	LOGI("str_vertex %s",strVertex.c_str());

	const char* CircleVertexShader = strVertex.c_str();

	string strFrag=FileUtil::loadShaderStr("CommonTex.frag");
//	LOGI("str_frag %s",strFrag.c_str());

	const char* CircleFragmentShader = strFrag.c_str();

    mProgram = ShaderUtil::createProgram(CircleVertexShader, CircleFragmentShader);
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

void Circle::refresh()
{
//	LOGI("circle0");
	//�������ݳ�ʼ��
	int count=0;
	int stCount=0;
	int normalCount =0;
	float angdegSpan=360.0f/this->m_n;//ÿ�������ζ���
//	LOGI("circle1");
	for(float angdeg=0;ceil(angdeg)<360;angdeg+=angdegSpan)
	{
//		LOGI("circle2");
		double angrad=angdeg/360.0*(2*MY_PI);//��ǰ����		toRadians(angdeg);
		double angradNext=(angdeg+angdegSpan)/360.0*(2*MY_PI);;//��һ����			toRadians(angdeg+angdegSpan)
		//���ĵ�
		m_vertices[count++]=0 + m_pos[m_index].x();//��������
		m_vertices[count++]=m_pos[m_index].y();
		m_vertices[count++]=0 + m_pos[m_index].z();

		m_normal[normalCount++] = 0;//����������
		m_normal[normalCount++] = 1;
		m_normal[normalCount++] = 0;

		m_textures[stCount++]=0.5f;//st����
		m_textures[stCount++]=0.5f;
		//��ǰ��
		m_vertices[count++]=(float) (-m_r*sin(angrad)) + m_pos[m_index].x();//��������
		m_vertices[count++]=m_pos[m_index].y();
		m_vertices[count++]=(float) (m_r*cos(angrad)) + m_pos[m_index].z();

		m_normal[normalCount++] = 0;//����������
		m_normal[normalCount++] = 1;
		m_normal[normalCount++] = 0;

		m_textures[stCount++]=(float) (0.5f-0.5f*sin(angrad));//st����
		m_textures[stCount++]=(float) (0.5f-0.5f*cos(angrad));
		//��һ��
		m_vertices[count++]=(float) (-m_r*sin(angradNext)) + m_pos[m_index].x();//��������
		m_vertices[count++]=m_pos[m_index].y();
		m_vertices[count++]=(float) (m_r*cos(angradNext)) + m_pos[m_index].z();

		m_normal[normalCount++] = 0;//����������
		m_normal[normalCount++] = 1;
		m_normal[normalCount++] = 0;

		m_textures[stCount++]=(float) (0.5f-0.5f*sin(angradNext));//st����
		m_textures[stCount++]=(float) (0.5f-0.5f*cos(angradNext));

//		LOGI("circle3");
	}
	mVertexBuffer = &m_vertices[0];//���������ݻ��帳ֵ
	mTextureBuffer = &m_textures[0];//���������ݻ��帳ֵ
	mNormalBuffer = &m_normal[0];//�����������ݻ��帳ֵ
}

void Circle::drawSelf(int texIdIn)
{
	if(needRefresh)
	{
		refresh();
	}

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

    glDrawArrays(GL_TRIANGLES, 0, m_vCount);//���Ʋ���GL_TRIANGLES��ʽ��0��vCount������		����

    glDisableVertexAttribArray(maPositionHandle);//�����������ж���λ����������id
    glDisableVertexAttribArray(maTexCoorHandle); //�����������ж���������������id		�رգ���
    glDisableVertexAttribArray(maNormalHandle);

	MatrixState::popMatrix();//�ָ��ֳ�
}
