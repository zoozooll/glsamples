#include "ConeSide.h"
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

ConeSide::ConeSide(
			int texIdIn,
			float r,//�뾶
			int n,//�еķ���
			float h//Բ���ĸ߶�
			)
{
	float angdegSpan=360.0f/n;//ÿ�������ζ���
	vCount = n * 3;//��������	���ظ�ֵ   * 2
	texId = texIdIn;
	float* vertices=new float[vCount*3];//��������
	float* normals=new float[vCount*3];//����������
	float* textures=new float[vCount*2];//��������S��T����ֵ����

	btVector3* vertices_repeat = new btVector3[vCount];//�������飨���ظ����㣩
	int* index_vertices_repeat = new int[vCount];//�������飨���ظ����㣩����������
	int vrCount = 0;
	int vrCount_noRepeat = 1;
	int ivrCount = 0;

	//�������ݳ�ʼ��
	int count=0;
	int stCount=0;
	int normalCount = 0;
	for(float angdeg=0;ceil(angdeg)<360;angdeg+=angdegSpan)
	{
		double angrad=angdeg/360.0*(2*SIMD_PI);//��ǰ����		toRadians(angdeg);
		double angradNext=(angdeg+angdegSpan)/360.0*(2*SIMD_PI);;//��һ����			toRadians(angdeg+angdegSpan)
		//���ĵ�
		vertices[count++]=0;//��������
		vertices[count++]=h/2;
		vertices[count++]=0;

		vertices_repeat[vrCount++].setValue(0, h/2, 0);
		index_vertices_repeat[ivrCount++] = 0;

		textures[stCount++]=0.5f;//st����
		textures[stCount++]=0.5f;
		//��ǰ��
		vertices[count++]=(float) (-r*sin(angrad));//��������
		vertices[count++]=-h/2;
		vertices[count++]=(float) (r*cos(angrad));

		vertices_repeat[vrCount++].setValue((float) (-r*sin(angrad)), -h/2, (float) (r*cos(angrad)));
		index_vertices_repeat[ivrCount++] = vrCount_noRepeat;
		vrCount_noRepeat++;

		textures[stCount++]=(float) (0.5f-0.5f*sin(angrad));//st����
		textures[stCount++]=(float) (0.5f-0.5f*cos(angrad));
		//��һ��
		vertices[count++]=(float) (-r*sin(angradNext));//��������
		vertices[count++]=-h/2;
		vertices[count++]=(float) (r*cos(angradNext));

		vertices_repeat[vrCount++].setValue((float) (-r*sin(angradNext)), -h/2, (float) (r*cos(angradNext)));
		index_vertices_repeat[ivrCount++] = vrCount_noRepeat;

		textures[stCount++]=(float) (0.5f-0.5f*sin(angradNext));//st����
		textures[stCount++]=(float) (0.5f-0.5f*cos(angradNext));

		normals[normalCount++] = 0;//����������
		normals[normalCount++] = 1;
		normals[normalCount++] = 0;

		normals[normalCount++] = 0;//����������
		normals[normalCount++] = 1;
		normals[normalCount++] = 0;

		normals[normalCount++] = 0;//����������
		normals[normalCount++] = 1;
		normals[normalCount++] = 0;
	}
	mVertexBuffer = &vertices[0];//���������ݻ��帳ֵ
	mTextureBuffer = &textures[0];//���������ݻ��帳ֵ

	index_vertices_repeat[vCount-1] = 1;
	//���㷨��������
	btVector3* vertices_normal =
			method(vertices_repeat, vCount, index_vertices_repeat, vCount, index_vertices_repeat[vCount-2]+1);//vrCount_noRepeat-1
	//��btVector3����ת��Ϊ��Ӧ��float���͵�����
	float* vertices_normal_float = new float[vCount * 3];
	int index_vertices_normal_float = 0;
	for(int i = 0; i<vCount; i++)
	{
		vertices_normal_float[index_vertices_normal_float++] = vertices_normal[i].x();
		vertices_normal_float[index_vertices_normal_float++] = vertices_normal[i].y();
		vertices_normal_float[index_vertices_normal_float++] = vertices_normal[i].z();
	}
	mNormalBuffer = &vertices_normal_float[0];//�����㷨�������ݻ��帳ֵ

//	mNormalBuffer = &vertices[0];//�����������ݻ��帳ֵ

	initShader();
}

void ConeSide::initShader()
{
//	LOGI("initShader");

	string strVertex=FileUtil::loadShaderStr("CommonTex.vert");
//	LOGI("str_vertex %s",strVertex.c_str());

	const char* ConeSideVertexShader = strVertex.c_str();

	string strFrag=FileUtil::loadShaderStr("CommonTex.frag");
//	LOGI("str_frag %s",strFrag.c_str());

	const char* ConeSideFragmentShader = strFrag.c_str();

    mProgram = ShaderUtil::createProgram(ConeSideVertexShader, ConeSideFragmentShader);
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

void ConeSide::drawSelf(int texIdIn)
{
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
btVector3* ConeSide::method(
		btVector3* vertices,//��������(���ظ�ֵ��)
		int nums_vertices,//��������ĳ���
		int* index_vertices,//�����������������
		int nums_indexVertices,//�����������������ĳ���	ע�⣺Ӧ����nums_vertices��ͬ
		int nums_noRepeat//������Ŀ�����ظ��������Ŀ
		)
{
	btVector3* vertices_Normal = new btVector3[nums_vertices];//��������������
	btVector3* tempVecticsNormal = new btVector3[nums_noRepeat];//������ʱ���㷨��������,�����ظ�����
	//��ʼ��������
	for(int i = 0; i<nums_vertices; i++)
	{
		vertices_Normal[i].setZero();
//		LOGI("%f,%f,%f",vertices_Normal[i].x(),vertices_Normal[i].y(),vertices_Normal[i].z());
	}
	for(int i = 0; i<nums_noRepeat; i++)
	{
		tempVecticsNormal[i].setZero();
	}

	int nums_triangle = nums_vertices / 3;//��ȡ����������
	for(int i = 0; i<nums_triangle; i++)//�������е�������
	{
		//��ȡһ�������ε���������
		btVector3 btv1 = vertices[i*3+0];
		btVector3 btv2 = vertices[i*3+1];
		btVector3 btv3 = vertices[i*3+2];

		//��ȡ�������ε�����ֵ
		int index_btv1 = index_vertices[i*3+0];
		int index_btv2 = index_vertices[i*3+1];
		int index_btv3 = index_vertices[i*3+2];

		//��ȡ���������������ߵ�����
		btVector3 btv1_btv2 = btv1 - btv2;
		btVector3 btv1_btv3 = btv1 - btv3;

		//���㵱ǰ�����εķ�����
		btVector3 tempNormal = btv1_btv2.cross(btv1_btv3);//����

//		LOGI("%f,%f,%f",tempNormal.x(),tempNormal.y(),tempNormal.z());

		//
		tempVecticsNormal[index_btv1] += tempNormal;
		tempVecticsNormal[index_btv2] += tempNormal;
		tempVecticsNormal[index_btv3] += tempNormal;

//		//��ÿ������ķ�������ֵ
//		vertices_Normal[i*3+0] = tempVecticsNormal[index_btv1];
//		vertices_Normal[i*3+1] = tempVecticsNormal[index_btv2];
//		vertices_Normal[i*3+2] = tempVecticsNormal[index_btv3];
	}
	for(int i = 0; i<nums_triangle; i++)//�������е�������
	{
		//��ÿ������ķ�������ֵ
		vertices_Normal[i*3+0] = tempVecticsNormal[index_vertices[i*3+0]];
		vertices_Normal[i*3+1] = tempVecticsNormal[index_vertices[i*3+1]];
		vertices_Normal[i*3+2] = tempVecticsNormal[index_vertices[i*3+2]];
	}
	for(int i = 0; i<nums_vertices; i++)//�������еĶ�����
	{
		vertices_Normal[i] = vertices_Normal[i].normalized();//��ÿ������ķ�������λ��
		vertices_Normal[i].setX(-1.0f * vertices_Normal[i].x());
		vertices_Normal[i].setY(-1.0f * vertices_Normal[i].y());
		vertices_Normal[i].setZ(-1.0f * vertices_Normal[i].z());

//		LOGI("%f,%f,%f",vertices_Normal[i].x(),vertices_Normal[i].y(),vertices_Normal[i].z());
	}
	return vertices_Normal;
}
