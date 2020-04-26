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
			float r,//半径
			int n,//切的份数
			float h//圆柱的高度
			)
{
	float angdegSpan=360.0f/n;//每个三角形顶角
	vCount = n * 3;//顶点总数	有重复值   * 2
	texId = texIdIn;
	float* vertices=new float[vCount*3];//坐标数据
	float* normals=new float[vCount*3];//法向量数据
	float* textures=new float[vCount*2];//顶点纹理S、T坐标值数组

	btVector3* vertices_repeat = new btVector3[vCount];//顶点数组（有重复顶点）
	int* index_vertices_repeat = new int[vCount];//顶点数组（有重复顶点）的索引数组
	int vrCount = 0;
	int vrCount_noRepeat = 1;
	int ivrCount = 0;

	//坐标数据初始化
	int count=0;
	int stCount=0;
	int normalCount = 0;
	for(float angdeg=0;ceil(angdeg)<360;angdeg+=angdegSpan)
	{
		double angrad=angdeg/360.0*(2*SIMD_PI);//当前弧度		toRadians(angdeg);
		double angradNext=(angdeg+angdegSpan)/360.0*(2*SIMD_PI);;//下一弧度			toRadians(angdeg+angdegSpan)
		//中心点
		vertices[count++]=0;//顶点坐标
		vertices[count++]=h/2;
		vertices[count++]=0;

		vertices_repeat[vrCount++].setValue(0, h/2, 0);
		index_vertices_repeat[ivrCount++] = 0;

		textures[stCount++]=0.5f;//st坐标
		textures[stCount++]=0.5f;
		//当前点
		vertices[count++]=(float) (-r*sin(angrad));//顶点坐标
		vertices[count++]=-h/2;
		vertices[count++]=(float) (r*cos(angrad));

		vertices_repeat[vrCount++].setValue((float) (-r*sin(angrad)), -h/2, (float) (r*cos(angrad)));
		index_vertices_repeat[ivrCount++] = vrCount_noRepeat;
		vrCount_noRepeat++;

		textures[stCount++]=(float) (0.5f-0.5f*sin(angrad));//st坐标
		textures[stCount++]=(float) (0.5f-0.5f*cos(angrad));
		//下一点
		vertices[count++]=(float) (-r*sin(angradNext));//顶点坐标
		vertices[count++]=-h/2;
		vertices[count++]=(float) (r*cos(angradNext));

		vertices_repeat[vrCount++].setValue((float) (-r*sin(angradNext)), -h/2, (float) (r*cos(angradNext)));
		index_vertices_repeat[ivrCount++] = vrCount_noRepeat;

		textures[stCount++]=(float) (0.5f-0.5f*sin(angradNext));//st坐标
		textures[stCount++]=(float) (0.5f-0.5f*cos(angradNext));

		normals[normalCount++] = 0;//法向量坐标
		normals[normalCount++] = 1;
		normals[normalCount++] = 0;

		normals[normalCount++] = 0;//法向量坐标
		normals[normalCount++] = 1;
		normals[normalCount++] = 0;

		normals[normalCount++] = 0;//法向量坐标
		normals[normalCount++] = 1;
		normals[normalCount++] = 0;
	}
	mVertexBuffer = &vertices[0];//给顶点数据缓冲赋值
	mTextureBuffer = &textures[0];//给纹理数据缓冲赋值

	index_vertices_repeat[vCount-1] = 1;
	//顶点法向量数组
	btVector3* vertices_normal =
			method(vertices_repeat, vCount, index_vertices_repeat, vCount, index_vertices_repeat[vCount-2]+1);//vrCount_noRepeat-1
	//将btVector3类型转化为对应的float类型的数组
	float* vertices_normal_float = new float[vCount * 3];
	int index_vertices_normal_float = 0;
	for(int i = 0; i<vCount; i++)
	{
		vertices_normal_float[index_vertices_normal_float++] = vertices_normal[i].x();
		vertices_normal_float[index_vertices_normal_float++] = vertices_normal[i].y();
		vertices_normal_float[index_vertices_normal_float++] = vertices_normal[i].z();
	}
	mNormalBuffer = &vertices_normal_float[0];//给顶点法向量数据缓冲赋值

//	mNormalBuffer = &vertices[0];//给法向量数据缓冲赋值

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

void ConeSide::drawSelf(int texIdIn)
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
btVector3* ConeSide::method(
		btVector3* vertices,//顶点数组(有重复值的)
		int nums_vertices,//顶点数组的长度
		int* index_vertices,//顶点数组的索引数组
		int nums_indexVertices,//顶点数组的索引数组的长度	注意：应该与nums_vertices相同
		int nums_noRepeat//顶点数目，无重复顶点的数目
		)
{
	btVector3* vertices_Normal = new btVector3[nums_vertices];//创建法向量数组
	btVector3* tempVecticsNormal = new btVector3[nums_noRepeat];//创建临时顶点法向量数组,不含重复顶点
	//初始化法向量
	for(int i = 0; i<nums_vertices; i++)
	{
		vertices_Normal[i].setZero();
//		LOGI("%f,%f,%f",vertices_Normal[i].x(),vertices_Normal[i].y(),vertices_Normal[i].z());
	}
	for(int i = 0; i<nums_noRepeat; i++)
	{
		tempVecticsNormal[i].setZero();
	}

	int nums_triangle = nums_vertices / 3;//获取三角形总数
	for(int i = 0; i<nums_triangle; i++)//遍历所有的三角形
	{
		//获取一个三角形的三个顶点
		btVector3 btv1 = vertices[i*3+0];
		btVector3 btv2 = vertices[i*3+1];
		btVector3 btv3 = vertices[i*3+2];

		//获取该三角形的索引值
		int index_btv1 = index_vertices[i*3+0];
		int index_btv2 = index_vertices[i*3+1];
		int index_btv3 = index_vertices[i*3+2];

		//获取三角形其中两条边的向量
		btVector3 btv1_btv2 = btv1 - btv2;
		btVector3 btv1_btv3 = btv1 - btv3;

		//计算当前三角形的法向量
		btVector3 tempNormal = btv1_btv2.cross(btv1_btv3);//求叉积

//		LOGI("%f,%f,%f",tempNormal.x(),tempNormal.y(),tempNormal.z());

		//
		tempVecticsNormal[index_btv1] += tempNormal;
		tempVecticsNormal[index_btv2] += tempNormal;
		tempVecticsNormal[index_btv3] += tempNormal;

//		//给每个顶点的法向量赋值
//		vertices_Normal[i*3+0] = tempVecticsNormal[index_btv1];
//		vertices_Normal[i*3+1] = tempVecticsNormal[index_btv2];
//		vertices_Normal[i*3+2] = tempVecticsNormal[index_btv3];
	}
	for(int i = 0; i<nums_triangle; i++)//遍历所有的三角形
	{
		//给每个顶点的法向量赋值
		vertices_Normal[i*3+0] = tempVecticsNormal[index_vertices[i*3+0]];
		vertices_Normal[i*3+1] = tempVecticsNormal[index_vertices[i*3+1]];
		vertices_Normal[i*3+2] = tempVecticsNormal[index_vertices[i*3+2]];
	}
	for(int i = 0; i<nums_vertices; i++)//遍历所有的顶点数
	{
		vertices_Normal[i] = vertices_Normal[i].normalized();//将每个顶点的法向量单位化
		vertices_Normal[i].setX(-1.0f * vertices_Normal[i].x());
		vertices_Normal[i].setY(-1.0f * vertices_Normal[i].y());
		vertices_Normal[i].setZ(-1.0f * vertices_Normal[i].z());

//		LOGI("%f,%f,%f",vertices_Normal[i].x(),vertices_Normal[i].y(),vertices_Normal[i].z());
	}
	return vertices_Normal;
}
