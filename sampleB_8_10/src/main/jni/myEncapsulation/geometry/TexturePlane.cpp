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
		float halfWidth, //半宽
		float halfHeight, //半高
		float offset, //位移量
		float UNIT_SIZE//单位长度
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
	//  0----1    顺序为  0,1,2  0,2,3
    vCount = 6;
    float *vertices=new float[vCount*3];
	int count=0;//顶点计数器

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
	count=0;//顶点计数器
	for(int i = 0; i<vCount; i++)
	{
		normal[count++] = 0;
		normal[count++] = 1;
		normal[count++] = 0;
	}
	mNormalBuffer = &normal[0];

    float *textures=new float[vCount*2];
	count=0;//顶点计数器

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

	//基于顶点着色器与片元着色器创建程序
    mProgram = ShaderUtil::createProgram(SimpleVertexShader, SimpleFragmentShader);
    //获取程序中顶点位置属性引用id
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    //获取程序中顶点经纬度属性引用id
    maTexCoorHandle=glGetAttribLocation(mProgram, "aTexCoor");
    //获取程序中总变换矩阵引用id
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    //获取位置、旋转变换矩阵引用id
    muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    //获取程序中摄像机位置引用id
    maCameraHandle=glGetUniformLocation(mProgram, "uCamera");
    //获取纹理的引用
    mTexHandle=glGetUniformLocation(mProgram, "sTexture");

    //获取顶点法向量属性的引用
    maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
    //获取程序中光源的位置的引用
    maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");

    mFragK = glGetUniformLocation(mProgram, "k");
}

void TexturePlane::drawSelf(const GLint texId)
{
	//制定使用某套shader程序
	glUseProgram(mProgram);
	//将最终变换矩阵传入shader程序
	glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());
	//将位置、旋转变换矩阵传入shader程序
	glUniformMatrix4fv(muMMatrixHandle, 1, 0, MatrixState::getMMatrix());
	//将摄像机位置传入shader程序
	glUniform3fv(maCameraHandle, 1, MatrixState::cameraFB);
	//将光源位置传入着色器程序
	glUniform3fv(maLightLocationHandle, 1, MatrixState::lightPositionFBSun);

	//为画笔指定顶点位置数据
	glVertexAttribPointer
	(
		maPositionHandle,
		3,
		GL_FLOAT,
		GL_FALSE,
		3*4,
		mVertexBuffer
	);
	//为画笔指定顶点纹理坐标数据
	glVertexAttribPointer
	(
		maTexCoorHandle,
		2,
		GL_FLOAT,
		GL_FALSE,
		2*4,
		mTextureBuffer
	);
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

	//允许顶点位置数据数组
	glEnableVertexAttribArray(maPositionHandle);
	glEnableVertexAttribArray(maTexCoorHandle);
	glEnableVertexAttribArray(maNormalHandle);//handle 处置

	//绑定纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);
	glUniform1i(mTexHandle, 0);
	glUniform1f(mFragK, 0);//mFlagK 为0时表示正常纹理   为1时表示有红色值影响

	//绘制三角形
	glDrawArrays(GL_TRIANGLES, 0, vCount);

    glDisableVertexAttribArray(maPositionHandle);
    glDisableVertexAttribArray(maTexCoorHandle);
    glDisableVertexAttribArray(maNormalHandle);
}
