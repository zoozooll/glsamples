#include "AnyShapeNoTexture.h"
#include "..\ShaderUtil.h"
#include "..\MatrixState.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

#include <stdio.h>
#include <stdlib.h>
#include "..\FileUtil.h"

#include <string>
using namespace std;

AnyShapeNoTexture::AnyShapeNoTexture(
		float *vertices,//顶点*3坐标数组    有重复顶点的
		int m_numsVer
		)
{
	this->vCount = m_numsVer/3;//顶点总数  含重复顶点
	this->vCountLine = vCount * 2;//线 数量
	this->colors = new float[vCountLine * 4];//每个顶点对于着四个颜色信息
	this->m_vertices = vertices;
	this->m_verticesLine = new float[vCountLine*3];

//	this->vCount = m_numsVer/3;//顶点总数  含重复顶点
//	this->vCountLine = vCount;//线 数量
//	this->colors = new float[vCount * 4*2];//每个顶点对于着四个颜色信息
//	this->m_vertices = vertices;
//	this->m_verticesLine = new float[vCountLine*3*2];

	initShader();
}


void AnyShapeNoTexture::initShader()
{
	//	LOGI("initShader");

		string strVertex=FileUtil::loadShaderStr("CommonCol.vert");
	//	LOGI("str_vertex %s",strVertex.c_str());

		const char* ClothVertexShader = strVertex.c_str();

		string strFrag=FileUtil::loadShaderStr("CommonCol.frag");
//		LOGI("str_frag %s",strFrag.c_str());

		const char* ClothFragmentShader = strFrag.c_str();

	    mProgram = ShaderUtil::createProgram(ClothVertexShader, ClothFragmentShader);
	    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
	    maColorHandle = glGetAttribLocation(mProgram, "sColor");
	    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
}

void AnyShapeNoTexture::drawSelf()
{
	MatrixState::pushMatrix();//保护现场

	glUseProgram(mProgram);

	glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());

	glEnableVertexAttribArray(maPositionHandle);
	glEnableVertexAttribArray(maColorHandle);

	initVertex(1);
	initColor(1);
	glVertexAttribPointer(maPositionHandle, 3, GL_FLOAT, GL_FALSE, 3*4, mVertexBuffer);
	glVertexAttribPointer(maColorHandle, 4, GL_FLOAT, GL_FALSE, 4*4, mColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, vCount);

	initVertex(0);
	initColor(0);
	glVertexAttribPointer(maPositionHandle, 3, GL_FLOAT, GL_FALSE, 3*4, mVertexBuffer);
	glVertexAttribPointer(maColorHandle, 4, GL_FLOAT, GL_FALSE, 4*4, mColorBuffer);
	glDrawArrays(GL_LINES, 0, vCountLine);
	glLineWidth(2.0f);

	glDisableVertexAttribArray(maPositionHandle);
	glDisableVertexAttribArray(maColorHandle);

	MatrixState::popMatrix();//恢复现场
}

void AnyShapeNoTexture::initVertex(int f)
{
	if(f==1)
	{
		mVertexBuffer = &m_vertices[0];
	}
	else{
		int count=0;
		for(int i = 0; i<vCount / 3; i++)
		{
			m_verticesLine[count++] = m_vertices[(i*3+0)*3+0];
			m_verticesLine[count++] = m_vertices[(i*3+0)*3+1];
			m_verticesLine[count++] = m_vertices[(i*3+0)*3+2];

			m_verticesLine[count++] = m_vertices[(i*3+1)*3+0];
			m_verticesLine[count++] = m_vertices[(i*3+1)*3+1];
			m_verticesLine[count++] = m_vertices[(i*3+1)*3+2];

			m_verticesLine[count++] = m_vertices[(i*3+1)*3+0];
			m_verticesLine[count++] = m_vertices[(i*3+1)*3+1];
			m_verticesLine[count++] = m_vertices[(i*3+1)*3+2];

			m_verticesLine[count++] = m_vertices[(i*3+2)*3+0];
			m_verticesLine[count++] = m_vertices[(i*3+2)*3+1];
			m_verticesLine[count++] = m_vertices[(i*3+2)*3+2];

			m_verticesLine[count++] = m_vertices[(i*3+2)*3+0];
			m_verticesLine[count++] = m_vertices[(i*3+2)*3+1];
			m_verticesLine[count++] = m_vertices[(i*3+2)*3+2];

			m_verticesLine[count++] = m_vertices[(i*3+0)*3+0];
			m_verticesLine[count++] = m_vertices[(i*3+0)*3+1];
			m_verticesLine[count++] = m_vertices[(i*3+0)*3+2];
		}
		mVertexBuffer = &m_verticesLine[0];
	}
}

void AnyShapeNoTexture::initColor(int c)
{
	int count=0;							//顶点计数器
	int i=0;
	if(c==0)
	{
		for(i=0;i<vCountLine;++i)//
		{
			colors[count++] = 1;
			colors[count++] = 1;
			colors[count++] = 1;
			colors[count++] = 1;
		}
	}
	else
	{
		for(i=0;i<vCount;++i)
		{
			colors[count++] = 1;
			colors[count++] = 0.5;
			colors[count++] = 0;
			colors[count++] = 1;
		}
	}

	mColorBuffer = &colors[0];
}

