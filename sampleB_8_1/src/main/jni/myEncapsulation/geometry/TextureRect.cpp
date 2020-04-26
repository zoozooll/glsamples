#include "TextureRect.h"
#include "../ShaderUtil.h"
#include "../MatrixState.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

#include <stdio.h>
#include <stdlib.h>
#include "../FileUtil.h"

#include <string>
using namespace std;

TextureRect::TextureRect(
		float halfWidth, //半宽
		float halfHeight, //半高
		float offset, //位移量
		float UNIT_SIZE//单位长度
		)
{
    initVertexDataRectangle(halfWidth, halfHeight, offset, UNIT_SIZE);
    initShader();
}

void TextureRect::initVertexDataRectangle(float halfWidth, float halfHeight, float offset, float UNIT_SIZE)
{
	//  3----2
	//  |    |
	//	|	 |
	//  0----1    顺序为  0,1,2  0,2,3
    vCount = 6;
    float *vertices=new float[vCount*3];
	int count=0;//顶点计数器

	vertices[count++] = -halfWidth*UNIT_SIZE;
	vertices[count++] = -halfHeight*UNIT_SIZE;
	vertices[count++] = offset;

	vertices[count++] = halfWidth*UNIT_SIZE;
	vertices[count++] = -halfHeight*UNIT_SIZE;
	vertices[count++] = offset;

	vertices[count++] = halfWidth*UNIT_SIZE;
	vertices[count++] = halfHeight*UNIT_SIZE;
	vertices[count++] = offset;

	vertices[count++] = -halfWidth*UNIT_SIZE;
	vertices[count++] = -halfHeight*UNIT_SIZE;
	vertices[count++] = offset;

	vertices[count++] = halfWidth*UNIT_SIZE;
	vertices[count++] = halfHeight*UNIT_SIZE;
	vertices[count++] = offset;

	vertices[count++] = -halfWidth*UNIT_SIZE;
	vertices[count++] = halfHeight*UNIT_SIZE;
	vertices[count++] = offset;

	mVertexBuffer = &vertices[0];

    float *normal=new float[vCount*3];
	count=0;//顶点计数器
	for(int i = 0; i<vCount; i++)
	{
		normal[count++] = 0;
		normal[count++] = 0;
		normal[count++] = 1;
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


void TextureRect::initShader()
{
	string strVertex=FileUtil::loadShaderStr("CommonTex.vert");
	string strFrag=FileUtil::loadShaderStr("CommonTex.frag");

	const char* SimpleVertexShader = strVertex.c_str();
	const char* SimpleFragmentShader = strFrag.c_str();

	//基于顶点着色器与片元着色器创建程序
    mProgram = ShaderUtil::createProgram(SimpleVertexShader, SimpleFragmentShader);
    //获取程序中顶点位置属性引用
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    //获取程序中顶点纹理属性引用
    maTexCoorHandle=glGetAttribLocation(mProgram, "aTexCoor");
    //获取程序中总变换矩阵引用
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    //获取位置、旋转变换矩阵引用
    muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    //获取程序中摄像机位置引用
    maCameraHandle=glGetUniformLocation(mProgram, "uCamera");
    //获取纹理的引用
    mTexHandle=glGetUniformLocation(mProgram, "sTexture");

    //获取顶点法向量属性的引用
    maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
    //获取程序中光源的位置的引用
    maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");

    mFragK = glGetUniformLocation(mProgram, "k");
}

void TextureRect::drawSelf(const GLint texId)
{
	//指定使用某套着色器程序
	glUseProgram(mProgram);
	//将最终变换矩阵传入渲染管线
	glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());
	//将位置、旋转变换矩阵传入渲染管线
	glUniformMatrix4fv(muMMatrixHandle, 1, 0, MatrixState::getMMatrix());
	//将摄像机位置传入渲染管线
	glUniform3fv(maCameraHandle, 1, MatrixState::cameraFB);
	//将光源位置传入渲染管线
	glUniform3fv(maLightLocationHandle, 1, MatrixState::lightPositionFBSun);

	//将顶点位置数据传入渲染管线
	glVertexAttribPointer
	(
		maPositionHandle,
		3,
		GL_FLOAT,
		GL_FALSE,
		3*4,
		mVertexBuffer
	);
	//将顶点纹理坐标数据传入渲染管线
	glVertexAttribPointer
	(
		maTexCoorHandle,
		2,
		GL_FLOAT,
		GL_FALSE,
		2*4,
		mTextureBuffer
	);
	//为将顶点法向量数据传入渲染管线
	glVertexAttribPointer
	(
		maNormalHandle,
		3,
		GL_FLOAT,
		GL_FALSE,
		3*4,
		mNormalBuffer
	);

	glEnableVertexAttribArray(maPositionHandle);//启用顶点坐标数据数组
	glEnableVertexAttribArray(maTexCoorHandle);//启用顶点纹理坐标数据数组
	glEnableVertexAttribArray(maNormalHandle);//启用顶点法向量数据数组


	glActiveTexture(GL_TEXTURE0);//选择纹理单元
	glBindTexture(GL_TEXTURE_2D, texId);	//绑定纹理
	glUniform1i(mTexHandle, 0);
	glUniform1f(mFragK, mFlagK);//mFlagK 为0时表示正常纹理   为1时表示有红色值影响

	//绘制三角形
	glDrawArrays(GL_TRIANGLES, 0, vCount);

    glDisableVertexAttribArray(maPositionHandle);
    glDisableVertexAttribArray(maTexCoorHandle);
    glDisableVertexAttribArray(maNormalHandle);
}
