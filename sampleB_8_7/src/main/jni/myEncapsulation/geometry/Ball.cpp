#include "Ball.h"
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


Ball::Ball(
			int texIdIn,
			float r//半径
			)
{
	this->r = r;
//	this->texId = texIdIn;
	initVertexData();
	initShader();
}

float Ball::toRadians(float angle)
{
    return angle*3.1415926f/180;//试一下
}
//自动切分纹理产生纹理数组的方法
void Ball::generateTexCoor(int bw,int bh,float* tex){
    //float result[bw*bh*6*2];
    float sizew=1.0f/bw;//列数
    float sizeh=1.0f/bh;//行数
    int c=0;
    for(int i=0;i<bh;i++){
        for(int j=0;j<bw;j++){
            //每行列一个矩形，由两个三角形构成，共六个点，12个纹理坐标
            float s=j*sizew;
            float t=i*sizeh;
            tex[c++]=s;
            tex[c++]=t;
            tex[c++]=s;
            tex[c++]=t+sizeh;
            tex[c++]=s+sizew;
            tex[c++]=t;
            tex[c++]=s+sizew;
            tex[c++]=t;
            tex[c++]=s;
            tex[c++]=t+sizeh;
            tex[c++]=s+sizew;
            tex[c++]=t+sizeh;
    }}
}
void Ball::initVertexData()
{
	vCount=11664/3;//顶点的数量为坐标值数量的1/3，因为一个顶点有3个坐标
    //顶点坐标数据的初始化================begin============================
    const float angleSpan=10;//将球进行单位切分的角度
    float *vertices=new float[11664];//36*18*6*3
	int count=0;//顶点计数器

    for(float vAngle=90;vAngle>-90;vAngle=vAngle-angleSpan){//垂直方向angleSpan度一份
        for(float hAngle=360;hAngle>0;hAngle=hAngle-angleSpan){//水平方向angleSpan度一份
            //纵向横向各到一个角度后计算对应的此点在球面上的坐标
            double xozLength=r*cos(toRadians(vAngle));
            float x1=(float)(xozLength*cos(toRadians(hAngle)));
            float z1=(float)(xozLength*sin(toRadians(hAngle)));
            float y1=(float)(r*sin(toRadians(vAngle)));

            xozLength=r*cos(toRadians(vAngle-angleSpan));
            float x2=(float)(xozLength*cos(toRadians(hAngle)));
            float z2=(float)(xozLength*sin(toRadians(hAngle)));
            float y2=(float)(r*sin(toRadians(vAngle-angleSpan)));

            xozLength=r*cos(toRadians(vAngle-angleSpan));
            float x3=(float)(xozLength*cos(toRadians(hAngle-angleSpan)));
            float z3=(float)(xozLength*sin(toRadians(hAngle-angleSpan)));
            float y3=(float)(r*sin(toRadians(vAngle-angleSpan)));

            xozLength=r*cos(toRadians(vAngle));
            float x4=(float)(xozLength*cos(toRadians(hAngle-angleSpan)));
            float z4=(float)(xozLength*sin(toRadians(hAngle-angleSpan)));
            float y4=(float)(r*sin(toRadians(vAngle)));

        	vertices[count++]=x1;
        	vertices[count++]=y1;
        	vertices[count++]=z1;

        	vertices[count++]=x2;
        	vertices[count++]=y2;
        	vertices[count++]=z2;

        	vertices[count++]=x4;
        	vertices[count++]=y4;
        	vertices[count++]=z4;

        	vertices[count++]=x4;
        	vertices[count++]=y4;
        	vertices[count++]=z4;

        	vertices[count++]=x2;
        	vertices[count++]=y2;
        	vertices[count++]=z2;

        	vertices[count++]=x3;
        	vertices[count++]=y3;
        	vertices[count++]=z3;

    }}
	mVertexBuffer = &vertices[0];
	mNormalBuffer = &vertices[0];//给顶点法向量数据缓冲赋值

    float *textures=new float[vCount*2];
    generateTexCoor(36, 18, textures);
    mTextureBuffer = &textures[0];
}

void Ball::initShader()
{
//	LOGI("initShader");

	string strVertex=FileUtil::loadShaderStr("CommonTex.vert");
//	LOGI("str_vertex %s",strVertex.c_str());

	const char* BallVertexShader = strVertex.c_str();

	string strFrag=FileUtil::loadShaderStr("CommonTex.frag");
//	LOGI("str_frag %s",strFrag.c_str());

	const char* BallFragmentShader = strFrag.c_str();

    mProgram = ShaderUtil::createProgram(BallVertexShader, BallFragmentShader);
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

void Ball::drawSelf(int texIdIn)
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
