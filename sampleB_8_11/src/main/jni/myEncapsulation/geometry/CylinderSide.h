#ifndef _CylinderSide_H_
#define _CylinderSide_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

//Բ���Ĳ���
class CylinderSide
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

    int 		texId;			//����id
    float 		TRIANGLE_SIZE;	//�����εĵ�λ��С
    int vCount;//�������� ���ظ�
public:
    float mFlagK;//��������
	CylinderSide(
			int texIdIn,
			float r,//�뾶
			int n,//�зֵķ���
			float h//Բ���ĸ߶�
			);//��Ϊy���Բ�����
//	~CylinderSide();

    void initShader();
    void drawSelf(int texIdIn);
};

#endif
