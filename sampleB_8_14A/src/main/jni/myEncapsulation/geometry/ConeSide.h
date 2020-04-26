#ifndef _ConeSide_H_
#define _ConeSide_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../Bullet/LinearMath/btVector3.h"

//Բ���Ĳ���
class ConeSide
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
	ConeSide(
			int texIdIn,
			float r,//�뾶
			int n,//�зֵķ���
			float h//Բ���ĸ߶�
			);//��Ϊy���Բ�����
//	~ConeSide();

    void initShader();
    void drawSelf(int texIdIn);
    btVector3* method(
    		btVector3* vertices,//��������(���ظ�ֵ��)
    		int nums_vertices,//��������ĳ���
    		int* index_vertices,//�����������������
    		int nums_indexVertices,//�����������������ĳ���	ע�⣺Ӧ����nums_vertices��ͬ
    		int nums_noRepeat//������Ŀ�����ظ��������Ŀ
    		);
};

#endif
