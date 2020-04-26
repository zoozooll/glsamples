#ifndef _CylinderSideInclined_H_
#define _CylinderSideInclined_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include "../../Bullet/LinearMath/btVector3.h"

//Բ���Ĳ���
class CylinderSideInclined
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

    btVector3* pos;
    int index;
    float r;

	float* vertices;//��������
	float* normals;//����������
	float* textures;//��������S��T����ֵ����
	int n;

public:
    float mFlagK;//��������
	CylinderSideInclined(
			int texIdIn,
			float r,//�뾶
			int n,//����������
			btVector3* pos,
			int index
			);//��Ϊy���Բ�����
	CylinderSideInclined();
//	~CylinderSideInclined();

	void refresh();
    void initShader();
    void drawSelf(int texIdIn);
};

#endif
