#ifndef _AnyShapeNoTexture_H_
#define _AnyShapeNoTexture_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "../Bullet/LinearMath/btVector3.h"

class AnyShapeNoTexture
{
	GLuint mProgram;//�Զ�����Ⱦ���߳���id
	GLuint muMVPMatrixHandle;//�ܱ任��������id

    GLuint maColorHandle;
    GLuint maPositionHandle; //����λ����������id

    const GLvoid* mVertexBuffer;//�����������ݻ���
    const GLvoid* mColorBuffer;//���������������ݻ���

    float *colors;
    float *m_vertices;
    float *m_verticesLine;

    int vCount;//��������  Ӧ��Ϊ2
    int vCountLine;


public :
    AnyShapeNoTexture(
    		float *m_vertices,//����*3��������    ���ظ������
    		int m_numsVer
    		);

    void initColor(int c);
    void initVertex(int f);

    void initShader();
    void drawSelf();

};

#endif
