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
			float r//�뾶
			)
{
	this->r = r;
//	this->texId = texIdIn;
	initVertexData();
	initShader();
}

float Ball::toRadians(float angle)
{
    return angle*3.1415926f/180;//��һ��
}
//�Զ��з����������������ķ���
void Ball::generateTexCoor(int bw,int bh,float* tex){
    //float result[bw*bh*6*2];
    float sizew=1.0f/bw;//����
    float sizeh=1.0f/bh;//����
    int c=0;
    for(int i=0;i<bh;i++){
        for(int j=0;j<bw;j++){
            //ÿ����һ�����Σ������������ι��ɣ��������㣬12����������
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
	vCount=11664/3;//���������Ϊ����ֵ������1/3����Ϊһ��������3������
    //�����������ݵĳ�ʼ��================begin============================
    const float angleSpan=10;//������е�λ�зֵĽǶ�
    float *vertices=new float[11664];//36*18*6*3
	int count=0;//���������

    for(float vAngle=90;vAngle>-90;vAngle=vAngle-angleSpan){//��ֱ����angleSpan��һ��
        for(float hAngle=360;hAngle>0;hAngle=hAngle-angleSpan){//ˮƽ����angleSpan��һ��
            //����������һ���ǶȺ�����Ӧ�Ĵ˵��������ϵ�����
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
	mNormalBuffer = &vertices[0];//�����㷨�������ݻ��帳ֵ

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
    //��ȡ�����ж���λ����������id
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    //��ȡ�����ж���������������id
    maTexCoorHandle=glGetAttribLocation(mProgram, "aTexCoor");
    //��ȡ�������ܱ任��������id
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    //��ȡλ�á���ת�任��������id
    muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    //��ȡ�����������λ������id
    maCameraHandle=glGetUniformLocation(mProgram, "uCamera");
    //��ȡ���졢��ҹ������������
    mTexHandle=glGetUniformLocation(mProgram, "sTexture");

    //��ȡ���㷨�������Ե�����
    maNormalHandle = glGetAttribLocation(mProgram, "aNormal");
    //��ȡ�����й�Դ��λ�õ�����
    maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");

    mFragK = glGetUniformLocation(mProgram, "k");
    mFlagK = 0.0f;
}

void Ball::drawSelf(int texIdIn)
{
	MatrixState::pushMatrix();//�����ֳ�

    glUseProgram(mProgram);

    glUniformMatrix4fv(muMVPMatrixHandle, 1, 0, MatrixState::getFinalMatrix());
    //��λ�á���ת�任��������ɫ������
    glUniformMatrix4fv(muMMatrixHandle, 1, 0, MatrixState::getMMatrix());
    //�������λ�ô�����ɫ������
    glUniform3fv(maCameraHandle, 1, MatrixState::cameraFB);
	//����Դλ�ô�����ɫ������
	glUniform3fv(maLightLocationHandle, 1, MatrixState::lightPositionFBSun);

    //�����������ж���λ����������id,һ����3�������������ͣ�GL_FALSE��һ����ռ�ֽ������������ݵ��׵�ַ
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3*4, mVertexBuffer);
    //�����������ж���������������id,һ����2�������������ͣ�GL_FALSE��һ����ռ�ֽ���������������׵�ַ
    glVertexAttribPointer(maTexCoorHandle,2,GL_FLOAT,GL_FALSE,2*4,mTextureBuffer);
	//Ϊ����ָ�����㷨��������
	glVertexAttribPointer
	(
		maNormalHandle,
		3,
		GL_FLOAT,
		GL_FALSE,
		3*4,
		mNormalBuffer
	);

    glEnableVertexAttribArray(maPositionHandle);//�����������ж���λ����������id
    glEnableVertexAttribArray(maTexCoorHandle); //�����������ж���������������id		��������
    glEnableVertexAttribArray(maNormalHandle);//handle ����

    //������
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texIdIn);
    glUniform1i(mTexHandle, 0);
    glUniform1f(mFragK, mFlagK);//mFlagK Ϊ0ʱ��ʾ��������   Ϊ1ʱ��ʾ�к�ɫֵӰ��

    glDrawArrays(GL_TRIANGLES, 0, vCount);//���Ʋ���GL_TRIANGLES��ʽ��0��vCount������		����

    glDisableVertexAttribArray(maPositionHandle);//�����������ж���λ����������id
    glDisableVertexAttribArray(maTexCoorHandle); //�����������ж���������������id		�رգ���
    glDisableVertexAttribArray(maNormalHandle);

	MatrixState::popMatrix();//�ָ��ֳ�
}
