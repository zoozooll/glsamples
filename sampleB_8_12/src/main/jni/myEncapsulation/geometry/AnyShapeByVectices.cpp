#include "AnyShapeByVectices.h"
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

AnyShapeByVectices::AnyShapeByVectices(
		float* vertices_fl,//����*3����	���ظ�ֵ
		int numsVerticesBt,//����*3���������ظ�ֵ��
		int* indices,//��������
		int numsIndices,//��������
		float lightFlag// Ϊ1��ʾ˳ʱ��     Ϊ-1��ʾ��ʱ��
		)//������
{
	btVector3* verticesBt = floatToBtVector3(vertices_fl,numsVerticesBt);

	this->m_vertices_noRepeat = vertices_fl;
	this->m_vCount = numsIndices;//��������	���ظ�ֵ
	this->m_indices = indices;//��������
	this->m_lightFlag = lightFlag;
	this->m_vCount_noRepeat = numsVerticesBt / 3;
	this->update = true;//��Ϊ�������������ظ�ֵ�Ķ�������ָ�룬�����µ�ʱ������Ҫ���ظ�ֵ�Ķ������飬������initVNData��������Ҫ����

	//-------��������----first---------
	m_vertices_bt = new btVector3[m_vCount];
	for(int i = 0; i < m_vCount; i++)//������������
	{
		int index = m_indices[i];//��ȡ��ǰ����
		m_vertices_bt[i] = verticesBt[index];
	}
	m_vertices = btVector3ToFloat(m_vertices_bt, m_vCount);//�������飬�������� �������鳤�� nums*3
	mVertexBuffer = &m_vertices[0];//���������ݻ��帳ֵ
	//-------��������----end---------

	//-------����������----first---------
	m_normals = method(
			m_vertices_bt,//��������(�ظ�ֵ��)
			m_vCount,//��������ĳ���(�ظ�ֵ��)
			m_indices,//�����������������
			m_vCount//�����������������ĳ���		Ӧ����3�ı���
			);//���ص����鳤����nums_vertices*3

	mNormalBuffer = &m_normals[0];//�����������ݻ��帳ֵ
	//-------����������----end---------

	//-------������������----first---------
	float* textures = new float[m_vCount * 2];
	int textures_count = 0;
	for(int i = 0; i < m_vCount * 2.0f / 12.0f; i++)
	{
		textures[textures_count++] = 0;
		textures[textures_count++] = 1;

		textures[textures_count++] = 0;
		textures[textures_count++] = 0;

		textures[textures_count++] = 1;
		textures[textures_count++] = 1;

		textures[textures_count++] = 0;
		textures[textures_count++] = 0;

		textures[textures_count++] = 1;
		textures[textures_count++] = 0;

		textures[textures_count++] = 1;
		textures[textures_count++] = 1;
	}
	mTextureBuffer = &textures[0];//���������ݻ��帳ֵ
	//-------������������----end---------

	initShader();
}

AnyShapeByVectices::AnyShapeByVectices(
		float* vertices_fl,//����*3����	���ظ�ֵ
		int numsVerticesBt,//����*3���������ظ�ֵ��
		float lightFlag// Ϊ1��ʾ˳ʱ��     Ϊ-1��ʾ��ʱ��
		)//������
{
	this->m_vCount = numsVerticesBt / 3;//��������	���ظ�ֵ
	this->m_lightFlag = lightFlag;
	this->update = false;//��Ϊ�������������ظ�ֵ�Ķ�������ָ�룬������initVNData�����в���Ҫ����

	//-------��������----first---------
	this->m_vertices = vertices_fl;
	mVertexBuffer = &m_vertices[0];//���������ݻ��帳ֵ
	//-------��������----end---------

	//-------����������----first---------
	this->m_vertices_bt = floatToBtVector3(vertices_fl,m_vCount*3);
	initIndices();
	this->m_normals = method(
			m_vertices_bt,//��������(�ظ�ֵ��)
			m_vCount,//��������ĳ���(�ظ�ֵ��)
			m_indices,
			m_vCount
			);//���ص����鳤����nums_vertices*3
	mNormalBuffer = &m_normals[0];//�����������ݻ��帳ֵ
	//-------����������----end---------

	//-------������������----first---------
	float* textures = new float[m_vCount * 2];
	int textures_count = 0;
	for(int i = 0; i < m_vCount * 2.0f / 12.0f; i++)
	{
		textures[textures_count++] = 0;
		textures[textures_count++] = 1;

		textures[textures_count++] = 0;
		textures[textures_count++] = 0;

		textures[textures_count++] = 1;
		textures[textures_count++] = 1;

		textures[textures_count++] = 0;
		textures[textures_count++] = 0;

		textures[textures_count++] = 1;
		textures[textures_count++] = 0;

		textures[textures_count++] = 1;
		textures[textures_count++] = 1;
	}
	mTextureBuffer = &textures[0];//���������ݻ��帳ֵ
	//-------������������----end---------

	initShader();
}
void AnyShapeByVectices::initIndices()
{
	int index = 1;
	this->m_indices = new int[m_vCount];
	m_indices[0] = 0;

	for(int i = 1; i<m_vCount; i++)
	{
		int temp;
		bool flag = false;
		for(int j = i-1; j>=0; j--)
		{
			if(m_vertices_bt[i] == m_vertices_bt[j])
			{
				flag = true;
				temp = j;
				break;
			}
		}
		if(flag == true)
		{
			m_indices[i] = m_indices[temp];
		}
		else
		{
			m_indices[i] = index;
			index++;
		}
	}
	this->m_vCount_noRepeat = index;
}

void AnyShapeByVectices::initVNData()
{

	if(m_normals)
	{
		delete m_normals;
		m_normals = 0;
	}

	if(update)
	{
		//-------��������----first---------
		btVector3* verticesBt = floatToBtVector3(m_vertices_noRepeat,m_vCount_noRepeat*3);
		m_vertices_bt = new btVector3[m_vCount];
		for(int i = 0; i < m_vCount; i++)//������������
		{
			int index = m_indices[i];//��ȡ��ǰ����
			m_vertices_bt[i] = verticesBt[index];
		}
		if(m_vertices)
		{
			delete m_vertices;
			m_vertices = 0;
		}
		m_vertices = btVector3ToFloat(m_vertices_bt, m_vCount);//�������飬�������� �������鳤�� nums*3
		mVertexBuffer = &m_vertices[0];//���������ݻ��帳ֵ
		delete verticesBt;
		delete m_vertices_bt;
		//-------��������----end---------
	}

	//-------����������----first---------
	this->m_vertices_bt = floatToBtVector3(m_vertices,m_vCount*3);
	this->m_normals = method(
			m_vertices_bt,//��������(�ظ�ֵ��)
			m_vCount,//��������ĳ���(�ظ�ֵ��)
			m_indices,
			m_vCount
			);//���ص����鳤����nums_vertices*3
	delete m_vertices_bt;
	mNormalBuffer = &m_normals[0];//�����������ݻ��帳ֵ
	//-------����������----end---------


}

float* AnyShapeByVectices::method(
		btVector3* vertices,//��������(���ظ�ֵ��)
		int nums_vertices,//��������ĳ���
		int* index_vertices,//�����������������
		int nums_indexVertices//�����������������ĳ���	ע�⣺Ӧ����nums_vertices��ͬ
		)
{
	btVector3* vertices_Normal = new btVector3[nums_vertices];//��������������
	btVector3* tempVecticsNormal = new btVector3[m_vCount_noRepeat];//������ʱ���㷨��������,�����ظ�����
	//��ʼ��������
	for(int i = 0; i<nums_vertices; i++)
	{
		vertices_Normal[i].setZero();
	}
	for(int i = 0; i<m_vCount_noRepeat; i++)
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
//		LOGI("%d,%d,%d",index_btv1,index_btv2,index_btv3);

		//��ȡ���������������ߵ�����
		btVector3 btv1_btv2 = btv1 - btv2;
		btVector3 btv1_btv3 = btv1 - btv3;

		//���㵱ǰ�����εķ�����
		btVector3 tempNormal = btv1_btv2.cross(btv1_btv3);//����

		//��ÿ������ķ�������ֵ
		tempVecticsNormal[index_btv1] += tempNormal;
		tempVecticsNormal[index_btv2] += tempNormal;
		tempVecticsNormal[index_btv3] += tempNormal;

	}
	for(int i = 0; i<nums_triangle; i++)//�������е�������
	{
		//��ÿ������ķ�������ֵ
		vertices_Normal[i*3+0] = m_lightFlag * tempVecticsNormal[index_vertices[i*3+0]];
		vertices_Normal[i*3+1] = m_lightFlag * tempVecticsNormal[index_vertices[i*3+1]];
		vertices_Normal[i*3+2] = m_lightFlag * tempVecticsNormal[index_vertices[i*3+2]];
	}
	float* answer = btVector3ToFloat(vertices_Normal,nums_vertices);
	delete vertices_Normal;//��������������
	delete tempVecticsNormal;//������ʱ���㷨��������,�����ظ�����
	return answer;
}

void AnyShapeByVectices::initShader()
{
	string strVertex = FileUtil::loadShaderStr("CommonTex.vert");
	const char* anyShapeVertex = strVertex.c_str();

	string strFragment = FileUtil::loadShaderStr("CommonTex.frag");
	const char* anyShapeFragment = strFragment.c_str();

	mProgram = ShaderUtil::createProgram(anyShapeVertex, anyShapeFragment);
	muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
	muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
	muTexHandle = glGetUniformLocation(mProgram, "sTexture");
	muLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");
	muCameraHandle = glGetUniformLocation(mProgram, "uCamera");//�����λ������id

	maPositionHandle = glGetAttribLocation(mProgram, "aPosition");//����λ������id
	maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");//����������������id
	maNormalHandle = glGetAttribLocation(mProgram, "aNormal");//���㷨��������id

}

void AnyShapeByVectices::drawSelf(int texIdIn)
{
	initVNData();

	MatrixState::pushMatrix();//�����ֳ�

    glUseProgram(mProgram);

	glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());
	glUniformMatrix4fv(muMMatrixHandle, 1, 0, MatrixState::getMMatrix());
	glUniform3fv(muCameraHandle, 1, MatrixState::cameraFB);
	glUniform3fv(muLightLocationHandle, 1, MatrixState::lightPositionFBSun);

    //�����������ж���λ����������id,һ����3�������������ͣ�GL_FALSE��һ����ռ�ֽ������������ݵ��׵�ַ
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3*4, mVertexBuffer);
    //�����������ж���������������id,һ����2�������������ͣ�GL_FALSE��һ����ռ�ֽ���������������׵�ַ
    glVertexAttribPointer(maTexCoorHandle,2,GL_FLOAT,GL_FALSE,2*4,mTextureBuffer);
	//Ϊ����ָ�����㷨��������
	glVertexAttribPointer(maNormalHandle,3,GL_FLOAT,GL_FALSE,3*4,mNormalBuffer);

    glEnableVertexAttribArray(maPositionHandle);//�����������ж���λ����������id
    glEnableVertexAttribArray(maTexCoorHandle); //�����������ж���������������id		��������
    glEnableVertexAttribArray(maNormalHandle);//handle ����

    //������
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texIdIn);
    glUniform1i(muTexHandle, 0);

    glDrawArrays(GL_TRIANGLES, 0, m_vCount);//���Ʋ���GL_TRIANGLES��ʽ��0��vCount������		����

    glDisableVertexAttribArray(maPositionHandle);//�����������ж���λ����������id
    glDisableVertexAttribArray(maTexCoorHandle); //�����������ж���������������id		�رգ���
    glDisableVertexAttribArray(maNormalHandle);

	MatrixState::popMatrix();//�ָ��ֳ�
}



float* AnyShapeByVectices::btVector3ToFloat(btVector3* vertex, int nums)//�������飬��������
{
	//��������ĳ���Ϊ nums*3
	float* answer = new float[nums * 3];
	for(int i = 0; i < nums; i++)
	{
		answer[i*3+0] = vertex[i].x();
		answer[i*3+1] = vertex[i].y();
		answer[i*3+2] = vertex[i].z();
	}
	return answer;
}

btVector3* AnyShapeByVectices::floatToBtVector3(float* vertex, int nums)//�������飬��������
{
	//��������ĳ���Ϊ nums/3
	btVector3* answer = new btVector3[nums / 3];
	for(int i = 0; i < nums / 3; i++)
	{
		answer[i].setValue(vertex[i*3+0],vertex[i*3+1],vertex[i*3+2]);
	}
	return answer;
}
