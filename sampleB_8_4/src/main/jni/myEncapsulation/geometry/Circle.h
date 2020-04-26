#ifndef _Circle_H_
#define _Circle_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

class Circle
{
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



    int 		texId;			//����id
    float 		TRIANGLE_SIZE;	//�����εĵ�λ��С
    int vCount;//�������� ���ظ�
public:
    float mFlagK;//��������
	Circle(
			int texIdIn,
			float r,//�뾶
			int n,//Բ���и�ķ���    30
			float h//Բ�ĸ߶�    0
			);//xoz���ϵ�Բ   �ǳ��ϵ�Բ
//	~Circle();

    void initShader();
    void drawSelf(int texId);
};

#endif
