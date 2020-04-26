#include "MatrixState.h"
#include "Matrix.h"
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <math.h>

float MatrixState::currMatrix[16];//��ǰ�任����
float MatrixState::mProjMatrix[16];//4x4���� ͶӰ��
float MatrixState::mVMatrix[16];//�����λ�ó���9��������
float MatrixState::mMVPMatrix[16];//��ȡ����������ܱ任����
float MatrixState::mVMatrixForSpecFrame[16];//�����λ�ó���9��������
//�����任�����ջ
float MatrixState::mStack[10][16];
int MatrixState::stackTop=-1;
GLfloat* MatrixState::cameraFB=NULL;
GLfloat* MatrixState::lightPositionFBSun=NULL;


void MatrixState::setInitStack()//��ȡ���任��ʼ����
{
	Matrix::setIdentityM(currMatrix,0);
}

void MatrixState::pushMatrix()//�����任����
{
	stackTop++;
	for(int i=0;i<16;i++)
	{
		mStack[stackTop][i]=currMatrix[i];
	}
}

void MatrixState::popMatrix()//�ָ��任����
{
	for(int i=0;i<16;i++)
	{
		currMatrix[i]=mStack[stackTop][i];
	}
	stackTop--;
}

void MatrixState::copyMVMatrix()//����һ֡�ڵ����������
{
	for(int i=0;i<16;i++)
	{
		mVMatrixForSpecFrame[i]=mVMatrix[i];
	}
}

void MatrixState::translate(float x,float y,float z)//������xyz���ƶ�
{
	Matrix::translateM(currMatrix, 0, x, y, z);
}

void MatrixState::rotate(float angle,float x,float y,float z)//������xyz���ƶ�
{
	Matrix::rotateM(currMatrix,0,angle,x,y,z);
}

void MatrixState::scale(float x,float y,float z)//������xyz������
{
	Matrix::scaleM(currMatrix,0, x, y, z);
}

void MatrixState::setCamera//���������
(
 float cx,
 float cy,
 float cz,
 float tx,
 float ty,
 float tz,
 float upx,
 float upy,
 float upz
 )
{
	Matrix::setLookAtM
	(
	 mVMatrix,
	 0,
	 cx,
	 cy,
	 cz,

	 tx,
	 ty,
	 tz,

	 upx,
	 upy,
	 upz
	 );

    static float cameraLocation[3];//�����λ��
	cameraLocation[0]=cx;
	cameraLocation[1]=cy;
	cameraLocation[2]=cz;

    cameraFB = cameraLocation;
}

void MatrixState::setProjectFrustum//����͸��ͶӰ����
(
 float left,
 float right,
 float bottom,
 float top,
 float near,
 float far
 )
{
	Matrix::frustumM(mProjMatrix, 0, left, right, bottom, top, near, far);
}

float* MatrixState::getFinalMatrix()//��ȡ����������ܱ任����
{
	Matrix::multiplyMM(mMVPMatrix, 0, mVMatrixForSpecFrame, 0, currMatrix, 0);
	Matrix::multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);

	return mMVPMatrix;
}

float* MatrixState::getMMatrix() //��ȡ��������ı任����
{
    return &currMatrix[0];
}


float* MatrixState::getProjMatrix()
{
	return mProjMatrix;
}

float* MatrixState::getCaMatrix()
{
	return mVMatrix;
}

//���õƹ�λ�õķ���
void MatrixState::setLightLocation(float x,float y,float z)
{
    static float lightLocation[3];//�ƹ�λ��

	lightLocation[0]=x;
	lightLocation[1]=y;
	lightLocation[2]=z;

	lightPositionFBSun = lightLocation;
}


