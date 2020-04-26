#ifndef TextureRect__h
#define TextureRect__h

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

class TextureRect {
    GLuint mProgram;//�Զ�����Ⱦ���߳���id
    GLuint muMVPMatrixHandle;//�ܱ任��������id
    GLuint muMMatrixHandle;//λ�á���ת�任����
    GLuint mTexHandle;//���������������id

    GLuint mFragK;//���������������id

    GLuint maCameraHandle; //�����λ����������id
    GLuint maLightLocationHandle;//��Դλ����������
    GLuint maPositionHandle; //����λ����������id
    GLuint maTexCoorHandle; //��������������������id
    GLuint maNormalHandle;//���㷨������������id

    const GLvoid* mVertexBuffer;	//�����������ݻ���
    const GLvoid* mTextureBuffer;	//���������������ݻ���
    const GLvoid* mNormalBuffer;	//���㷨�������ݻ���

    int vCount;
    float UNIT_SIZE;
public:
    float mFlagK;//��������
    TextureRect(
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
