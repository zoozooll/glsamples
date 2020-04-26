#ifndef _TexturePlane_H_
#define _TexturePlane_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

class TexturePlane {
    GLuint mProgram;//�Զ�����Ⱦ���߳���id
    GLuint muMVPMatrixHandle;//�ܱ任��������
    GLuint muMMatrixHandle;//λ�á���ת�任����
    GLuint mTexHandle;//���������������

    GLuint mFragK;//���������������

    GLuint maCameraHandle; //�����λ����������
    GLuint maLightLocationHandle;//��Դλ����������
    GLuint maPositionHandle; //����λ����������
    GLuint maTexCoorHandle; //��������������������
    GLuint maNormalHandle;//���㷨������������

    const GLvoid* mVertexBuffer;	//�����������ݻ���
    const GLvoid* mTextureBuffer;	//���������������ݻ���
    const GLvoid* mNormalBuffer;	//���㷨�������ݻ���

    int vCount;
    float UNIT_SIZE;
public:
    float mFlagK;//��������
    TexturePlane(
    		float halfWidth, //���
    		float halfHeight, //���
    		float offset, //λ����
    		float UNIT_SIZE//��λ����
			);//xoy���ϵ�һ������
    void initVertexDataRectangle(
    		float halfWidth, //���
    		float halfHeight, //���
    		float offset, //λ����   һ��Ϊ0
    		float UNIT_SIZE//��λ����
    		);
    void initShader();
    void drawSelf(const GLint texId);
};

#endif
