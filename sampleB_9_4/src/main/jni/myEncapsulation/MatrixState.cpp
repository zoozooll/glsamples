#include "MatrixState.h"
#include "Matrix.h"
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <math.h>

float MatrixState::currMatrix[16];//当前变换矩阵
float MatrixState::mProjMatrix[16];//4x4矩阵 投影用
float MatrixState::mVMatrix[16];//摄像机位置朝向9参数矩阵
float MatrixState::mMVPMatrix[16];//获取具体物体的总变换矩阵
float MatrixState::mVMatrixForSpecFrame[16];//摄像机位置朝向9参数矩阵
//保护变换矩阵的栈
float MatrixState::mStack[10][16];
int MatrixState::stackTop=-1;
GLfloat* MatrixState::cameraFB=NULL;
GLfloat* MatrixState::lightPositionFBSun=NULL;


void MatrixState::setInitStack()//获取不变换初始矩阵
{
	Matrix::setIdentityM(currMatrix,0);
}

void MatrixState::pushMatrix()//保护变换矩阵
{
	stackTop++;
	for(int i=0;i<16;i++)
	{
		mStack[stackTop][i]=currMatrix[i];
	}
}

void MatrixState::popMatrix()//恢复变换矩阵
{
	for(int i=0;i<16;i++)
	{
		currMatrix[i]=mStack[stackTop][i];
	}
	stackTop--;
}

void MatrixState::copyMVMatrix()//用于一帧内的摄像机矩阵
{
	for(int i=0;i<16;i++)
	{
		mVMatrixForSpecFrame[i]=mVMatrix[i];
	}
}

void MatrixState::translate(float x,float y,float z)//设置沿xyz轴移动
{
	Matrix::translateM(currMatrix, 0, x, y, z);
}

void MatrixState::rotate(float angle,float x,float y,float z)//设置绕xyz轴移动
{
	Matrix::rotateM(currMatrix,0,angle,x,y,z);
}

void MatrixState::scale(float x,float y,float z)//设置沿xyz轴缩放
{
	Matrix::scaleM(currMatrix,0, x, y, z);
}

void MatrixState::setCamera//设置摄像机
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

    static float cameraLocation[3];//摄像机位置
	cameraLocation[0]=cx;
	cameraLocation[1]=cy;
	cameraLocation[2]=cz;

    cameraFB = cameraLocation;
}

void MatrixState::setProjectFrustum//设置透视投影参数
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

float* MatrixState::getFinalMatrix()//获取具体物体的总变换矩阵
{
	Matrix::multiplyMM(mMVPMatrix, 0, mVMatrixForSpecFrame, 0, currMatrix, 0);
	Matrix::multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);

	return mMVPMatrix;
}

float* MatrixState::getMMatrix() //获取具体物体的变换矩阵
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

//设置灯光位置的方法
void MatrixState::setLightLocation(float x,float y,float z)
{
    static float lightLocation[3];//灯光位置

	lightLocation[0]=x;
	lightLocation[1]=y;
	lightLocation[2]=z;

	lightPositionFBSun = lightLocation;
}


