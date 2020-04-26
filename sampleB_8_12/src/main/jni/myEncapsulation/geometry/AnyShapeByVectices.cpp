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
		float* vertices_fl,//顶点*3数组	无重复值
		int numsVerticesBt,//顶点*3总数（无重复值）
		int* indices,//索引数组
		int numsIndices,//索引总数
		float lightFlag// 为1表示顺时针     为-1表示逆时针
		)//三角形
{
	btVector3* verticesBt = floatToBtVector3(vertices_fl,numsVerticesBt);

	this->m_vertices_noRepeat = vertices_fl;
	this->m_vCount = numsIndices;//顶点总数	有重复值
	this->m_indices = indices;//索引数组
	this->m_lightFlag = lightFlag;
	this->m_vCount_noRepeat = numsVerticesBt / 3;
	this->update = true;//因为传进来的是无重复值的顶点数组指针，而更新的时候是需要有重复值的顶点数组，所以在initVNData方法中需要更新

	//-------顶点数据----first---------
	m_vertices_bt = new btVector3[m_vCount];
	for(int i = 0; i < m_vCount; i++)//遍历索引数组
	{
		int index = m_indices[i];//获取当前索引
		m_vertices_bt[i] = verticesBt[index];
	}
	m_vertices = btVector3ToFloat(m_vertices_bt, m_vCount);//顶点数组，顶点总数 返回数组长度 nums*3
	mVertexBuffer = &m_vertices[0];//给顶点数据缓冲赋值
	//-------顶点数据----end---------

	//-------法向量数据----first---------
	m_normals = method(
			m_vertices_bt,//顶点数组(重复值的)
			m_vCount,//顶点数组的长度(重复值的)
			m_indices,//顶点数组的索引数组
			m_vCount//顶点数组的索引数组的长度		应该是3的倍数
			);//返回的数组长度是nums_vertices*3

	mNormalBuffer = &m_normals[0];//给法向量数据缓冲赋值
	//-------法向量数据----end---------

	//-------顶点纹理数据----first---------
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
	mTextureBuffer = &textures[0];//给纹理数据缓冲赋值
	//-------顶点纹理数据----end---------

	initShader();
}

AnyShapeByVectices::AnyShapeByVectices(
		float* vertices_fl,//顶点*3数组	有重复值
		int numsVerticesBt,//顶点*3总数（有重复值）
		float lightFlag// 为1表示顺时针     为-1表示逆时针
		)//三角形
{
	this->m_vCount = numsVerticesBt / 3;//顶点总数	有重复值
	this->m_lightFlag = lightFlag;
	this->update = false;//因为传进来的是有重复值的顶点数组指针，所以在initVNData方法中不需要更新

	//-------顶点数据----first---------
	this->m_vertices = vertices_fl;
	mVertexBuffer = &m_vertices[0];//给顶点数据缓冲赋值
	//-------顶点数据----end---------

	//-------法向量数据----first---------
	this->m_vertices_bt = floatToBtVector3(vertices_fl,m_vCount*3);
	initIndices();
	this->m_normals = method(
			m_vertices_bt,//顶点数组(重复值的)
			m_vCount,//顶点数组的长度(重复值的)
			m_indices,
			m_vCount
			);//返回的数组长度是nums_vertices*3
	mNormalBuffer = &m_normals[0];//给法向量数据缓冲赋值
	//-------法向量数据----end---------

	//-------顶点纹理数据----first---------
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
	mTextureBuffer = &textures[0];//给纹理数据缓冲赋值
	//-------顶点纹理数据----end---------

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
		//-------顶点数据----first---------
		btVector3* verticesBt = floatToBtVector3(m_vertices_noRepeat,m_vCount_noRepeat*3);
		m_vertices_bt = new btVector3[m_vCount];
		for(int i = 0; i < m_vCount; i++)//遍历索引数组
		{
			int index = m_indices[i];//获取当前索引
			m_vertices_bt[i] = verticesBt[index];
		}
		if(m_vertices)
		{
			delete m_vertices;
			m_vertices = 0;
		}
		m_vertices = btVector3ToFloat(m_vertices_bt, m_vCount);//顶点数组，顶点总数 返回数组长度 nums*3
		mVertexBuffer = &m_vertices[0];//给顶点数据缓冲赋值
		delete verticesBt;
		delete m_vertices_bt;
		//-------顶点数据----end---------
	}

	//-------法向量数据----first---------
	this->m_vertices_bt = floatToBtVector3(m_vertices,m_vCount*3);
	this->m_normals = method(
			m_vertices_bt,//顶点数组(重复值的)
			m_vCount,//顶点数组的长度(重复值的)
			m_indices,
			m_vCount
			);//返回的数组长度是nums_vertices*3
	delete m_vertices_bt;
	mNormalBuffer = &m_normals[0];//给法向量数据缓冲赋值
	//-------法向量数据----end---------


}

float* AnyShapeByVectices::method(
		btVector3* vertices,//顶点数组(有重复值的)
		int nums_vertices,//顶点数组的长度
		int* index_vertices,//顶点数组的索引数组
		int nums_indexVertices//顶点数组的索引数组的长度	注意：应该与nums_vertices相同
		)
{
	btVector3* vertices_Normal = new btVector3[nums_vertices];//创建法向量数组
	btVector3* tempVecticsNormal = new btVector3[m_vCount_noRepeat];//创建临时顶点法向量数组,不含重复顶点
	//初始化法向量
	for(int i = 0; i<nums_vertices; i++)
	{
		vertices_Normal[i].setZero();
	}
	for(int i = 0; i<m_vCount_noRepeat; i++)
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
//		LOGI("%d,%d,%d",index_btv1,index_btv2,index_btv3);

		//获取三角形其中两条边的向量
		btVector3 btv1_btv2 = btv1 - btv2;
		btVector3 btv1_btv3 = btv1 - btv3;

		//计算当前三角形的法向量
		btVector3 tempNormal = btv1_btv2.cross(btv1_btv3);//求叉积

		//给每个顶点的法向量赋值
		tempVecticsNormal[index_btv1] += tempNormal;
		tempVecticsNormal[index_btv2] += tempNormal;
		tempVecticsNormal[index_btv3] += tempNormal;

	}
	for(int i = 0; i<nums_triangle; i++)//遍历所有的三角形
	{
		//给每个顶点的法向量赋值
		vertices_Normal[i*3+0] = m_lightFlag * tempVecticsNormal[index_vertices[i*3+0]];
		vertices_Normal[i*3+1] = m_lightFlag * tempVecticsNormal[index_vertices[i*3+1]];
		vertices_Normal[i*3+2] = m_lightFlag * tempVecticsNormal[index_vertices[i*3+2]];
	}
	float* answer = btVector3ToFloat(vertices_Normal,nums_vertices);
	delete vertices_Normal;//创建法向量数组
	delete tempVecticsNormal;//创建临时顶点法向量数组,不含重复顶点
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
	muCameraHandle = glGetUniformLocation(mProgram, "uCamera");//摄像机位置属性id

	maPositionHandle = glGetAttribLocation(mProgram, "aPosition");//顶点位置属性id
	maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");//顶点纹理坐标属性id
	maNormalHandle = glGetAttribLocation(mProgram, "aNormal");//顶点法向量属性id

}

void AnyShapeByVectices::drawSelf(int texIdIn)
{
	initVNData();

	MatrixState::pushMatrix();//保护现场

    glUseProgram(mProgram);

	glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());
	glUniformMatrix4fv(muMMatrixHandle, 1, 0, MatrixState::getMMatrix());
	glUniform3fv(muCameraHandle, 1, MatrixState::cameraFB);
	glUniform3fv(muLightLocationHandle, 1, MatrixState::lightPositionFBSun);

    //参数：程序中顶点位置属性引用id,一组有3个数，数的类型，GL_FALSE，一组所占字节数，顶点数据的首地址
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3*4, mVertexBuffer);
    //参数：程序中顶点纹理属性引用id,一组有2个数，数的类型，GL_FALSE，一组所占字节数，顶点纹理的首地址
    glVertexAttribPointer(maTexCoorHandle,2,GL_FLOAT,GL_FALSE,2*4,mTextureBuffer);
	//为画笔指定顶点法向量数据
	glVertexAttribPointer(maNormalHandle,3,GL_FLOAT,GL_FALSE,3*4,mNormalBuffer);

    glEnableVertexAttribArray(maPositionHandle);//参数：程序中顶点位置属性引用id
    glEnableVertexAttribArray(maTexCoorHandle); //参数：程序中顶点纹理属性引用id		开启？？
    glEnableVertexAttribArray(maNormalHandle);//handle 处置

    //绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texIdIn);
    glUniform1i(muTexHandle, 0);

    glDrawArrays(GL_TRIANGLES, 0, m_vCount);//绘制采用GL_TRIANGLES方式，0，vCount顶点数		？？

    glDisableVertexAttribArray(maPositionHandle);//参数：程序中顶点位置属性引用id
    glDisableVertexAttribArray(maTexCoorHandle); //参数：程序中顶点纹理属性引用id		关闭？？
    glDisableVertexAttribArray(maNormalHandle);

	MatrixState::popMatrix();//恢复现场
}



float* AnyShapeByVectices::btVector3ToFloat(btVector3* vertex, int nums)//顶点数组，顶点总数
{
	//返回数组的长度为 nums*3
	float* answer = new float[nums * 3];
	for(int i = 0; i < nums; i++)
	{
		answer[i*3+0] = vertex[i].x();
		answer[i*3+1] = vertex[i].y();
		answer[i*3+2] = vertex[i].z();
	}
	return answer;
}

btVector3* AnyShapeByVectices::floatToBtVector3(float* vertex, int nums)//顶点数组，顶点总数
{
	//返回数组的长度为 nums/3
	btVector3* answer = new btVector3[nums / 3];
	for(int i = 0; i < nums / 3; i++)
	{
		answer[i].setValue(vertex[i*3+0],vertex[i*3+1],vertex[i*3+2]);
	}
	return answer;
}
