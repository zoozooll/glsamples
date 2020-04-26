#ifndef _Ball_H_
#define _Ball_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

class Ball
{
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


    float r;
    int texId;			//����id
    int vCount;//�������� ���ظ�
public:
    float mFlagK;//��������
	Ball(
			int texIdIn,
			float r//�뾶
			);
//	~Ball();

    void initShader();
    void drawSelf(int texId);

    void initVertexData();
    float toRadians(float angle);
    void generateTexCoor(int bw,int bh,float* tex);
};

#endif
