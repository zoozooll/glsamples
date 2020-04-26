#include "CylinderSide.h"
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

CylinderSide::CylinderSide(
			int texIdIn,
			float r,//半径
			int n,//三角形总数
			float h//圆柱的高度
			)
{
	float angdegSpan=360.0f/n;//每个三角形顶角
	vCount = n * 2 * 3;//顶点总数	有重复值
	texId = texIdIn;
	float* vertices=new float[vCount*3];//坐标数据
	float* normals=new float[vCount*3];//法向量数据
	float* textures=new float[vCount*2];//顶点纹理S、T坐标值数组
	//坐标数据初始化
	int count=0;
	int stCount=0;
	int normalCount = 0;
	for(float angdeg=0;ceil(angdeg)<360;angdeg+=angdegSpan)
	{
		double angrad=angdeg/360.0*(2*MY_PI);//当前弧度		toRadians(angdeg);
		double angradNext=(angdeg+angdegSpan)/360.0*(2*MY_PI);;//下一弧度			toRadians(angdeg+angdegSpan)

		//底圆当前点---0
		vertices[count++]=(float) (-r*sin(angrad));
		vertices[count++]=-h/2;
		vertices[count++]=(float) (-r*cos(angrad));

		normals[normalCount++]=(float) (-r*sin(angrad));
		normals[normalCount++]=0;
		normals[normalCount++]=(float) (-r*cos(angrad));

		textures[stCount++]=(float) (angrad/(2*MY_PI));//st坐标
		textures[stCount++]=1;
		//顶圆下一点---3
		vertices[count++]=(float) (-r*sin(angradNext));
		vertices[count++]=h/2;
		vertices[count++]=(float) (-r*cos(angradNext));

		normals[normalCount++]=(float) (-r*sin(angradNext));
		normals[normalCount++]=0;
		normals[normalCount++]=(float) (-r*cos(angradNext));

		textures[stCount++]=(float) (angradNext/(2*MY_PI));//st坐标
		textures[stCount++]=0;
		//顶圆当前点---2
		vertices[count++]=(float) (-r*sin(angrad));
		vertices[count++]=h/2;
		vertices[count++]=(float) (-r*cos(angrad));

		normals[normalCount++]=(float)(-r*sin(angrad));
		normals[normalCount++]=0;
		normals[normalCount++]=(float)(-r*cos(angrad));

		textures[stCount++]=(float) (angrad/(2*MY_PI));//st坐标
		textures[stCount++]=0;

		//底圆当前点---0
		vertices[count++]=(float) (-r*sin(angrad));
		vertices[count++]=-h/2;
		vertices[count++]=(float) (-r*cos(angrad));

		normals[normalCount++]=(float)(-r*sin(angrad));
		normals[normalCount++]=0;
		normals[normalCount++]=(float)(-r*cos(angrad));

		textures[stCount++]=(float) (angrad/(2*MY_PI));//st坐标
		textures[stCount++]=1;
		//底圆下一点---1
		vertices[count++]=(float) (-r*sin(angradNext));
		vertices[count++]=-h/2;
		vertices[count++]=(float) (-r*cos(angradNext));

		normals[normalCount++]=(float)(-r*sin(angradNext));
		normals[normalCount++]=0;
		normals[normalCount++]=(float)(-r*cos(angradNext));

		textures[stCount++]=(float) (angradNext/(2*MY_PI));//st坐标
		textures[stCount++]=1;
		//顶圆下一点---3
		vertices[count++]=(float) (-r*sin(angradNext));
		vertices[count++]=h/2;
		vertices[count++]=(float) (-r*cos(angradNext));

		normals[normalCount++]=(float)(-r*sin(angradNext));
		normals[normalCount++]=0;
		normals[normalCount++]=(float)(-r*cos(angradNext));

		textures[stCount++]=(float) (angradNext/(2*MY_PI));//st坐标
		textures[stCount++]=0;
	}
	mVertexBuffer = &vertices[0];//给顶点数据缓冲赋值
	mTextureBuffer = &textures[0];//给纹理数据缓冲赋值
	mNormalBuffer = &normals[0];//给法向量数据缓冲赋值

	initShader();
}

void CylinderSide::initShader()
{

//	LOGI("initShader");

	string strVertex=FileUtil::loadShaderStr("CommonTex.vert");
//	LOGI("str_vertex %s",strVertex.c_str());

	const char* CylinderSideVertexShader = strVertex.c_str();

	string strFrag=FileUtil::loadShaderStr("CommonTex.frag");
//	LOGI("str_frag %s",strFrag.c_str());

	const char* CylinderSideFragmentShader = strFrag.c_str();

    mProgram = ShaderUtil::createProgram(CylinderSideVertexShader, CylinderSideFragmentShader);
    //获取程序中顶点位置属性引用id
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    //获取程序中顶点纹理属性引用id
    maTexCoorHandle=glGetAttribLocation(mProgram, "aTexCoor");
    //获取程序中总变换矩阵引用id
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    //获取位置、旋转变换矩阵引用id
    muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    //获取程序中摄像机位置引用id
    maCameraHandle=glGetUniformLocation(mProgram, "uCamera");
    //获取白天、黑夜两个纹理引用
    mTexHandle=glGetUniformLocation(mProgram, "sTexture");

    //获取顶点法向量属性的引用
    maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
    //获取程序中光源的位置的引用
    maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");

    mFragK = glGetUniformLocation(mProgram, "k");
    mFlagK = 0.0f;
}

void CylinderSide::drawSelf(int texIdIn)
{
	MatrixState::pushMatrix();//保护现场

    glUseProgram(mProgram);

    glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());
    //将位置、旋转变换矩阵传入着色器程序
    glUniformMatrix4fv(muMMatrixHandle, 1, 0, MatrixState::getMMatrix());
    //将摄像机位置传入着色器程序
    glUniform3fv(maCameraHandle, 1, MatrixState::cameraFB);
	//将光源位置传入着色器程序
	glUniform3fv(maLightLocationHandle, 1, MatrixState::lightPositionFBSun);


    //参数：程序中顶点位置属性引用id,一组有3个数，数的类型，GL_FALSE，一组所占字节数，顶点数据的首地址
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3*4, mVertexBuffer);
    //参数：程序中顶点纹理属性引用id,一组有2个数，数的类型，GL_FALSE，一组所占字节数，顶点纹理的首地址
    glVertexAttribPointer(maTexCoorHandle,2,GL_FLOAT,GL_FALSE,2*4,mTextureBuffer);
	//为画笔指定顶点法向量数据
	glVertexAttribPointer
	(
		maNormalHandle,
		3,
		GL_FLOAT,
		GL_FALSE,
		3*4,
		mNormalBuffer
	);

    glEnableVertexAttribArray(maPositionHandle);//参数：程序中顶点位置属性引用id
    glEnableVertexAttribArray(maTexCoorHandle); //参数：程序中顶点纹理属性引用id		开启？？
    glEnableVertexAttribArray(maNormalHandle);//handle 处置

    //绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texIdIn);
    glUniform1i(mTexHandle, 0);
    glUniform1f(mFragK, mFlagK);//mFlagK 为0时表示正常纹理   为1时表示有红色值影响

    glDrawArrays(GL_TRIANGLES, 0, vCount);//绘制采用GL_TRIANGLES方式，0，vCount顶点数		？？

    glDisableVertexAttribArray(maPositionHandle);//参数：程序中顶点位置属性引用id
    glDisableVertexAttribArray(maTexCoorHandle); //参数：程序中顶点纹理属性引用id		关闭？？
    glDisableVertexAttribArray(maNormalHandle);

	MatrixState::popMatrix();//恢复现场
}
