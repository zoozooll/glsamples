#ifndef _DrawLine_H_
#define _DrawLine_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "../Bullet/LinearMath/btVector3.h"

class DrawLine
{
	GLuint mProgram;//�Զ�����Ⱦ���߳���id
	GLuint muMVPMatrixHandle;//�ܱ任��������

    GLuint maColorHandle;
    GLuint maPositionHandle; //����λ����������

    const GLvoid* mVertexBuffer;//�����������ݻ���
    const GLvoid* mColorBuffer;//���������������ݻ���

    float *colors;
    float *vertices;

    int vCount;//��������  Ӧ��Ϊ2

public :
    DrawLine();
    DrawLine(
    		float *vertices//������������  ����Ӧ��Ϊ6
    		);
    DrawLine(
    		btVector3 vec0,//��һ������
    		btVector3 vec1//�ڶ�������
    		);

    void initShader();
    void drawSelf();
    void initVertex(
    		btVector3 vec0,//��һ������
    		btVector3 vec1//�ڶ�������
    		);
    void drawSelf(
    		btVector3 vec0,//��һ������
    		btVector3 vec1//�ڶ�������
    		);

};

#endif
