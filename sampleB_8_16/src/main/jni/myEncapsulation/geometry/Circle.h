#ifndef _Circle_H_
#define _Circle_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../../Bullet/LinearMath/btVector3.h"

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



    int m_vCount;//�������� ���ظ�
    bool needRefresh;
    btVector3* m_pos;//λ�� ָ��
    int m_index;//λ������ֵ  			m_pos[m_index]��ʾԲ���ĵ�λ��
    int m_texId;

	float* m_vertices;//��������
	float* m_normal;//����������
	float* m_textures;//��������S��T����ֵ����
	int m_n;//�зַ���
	float m_r;//�뾶
public:
    float mFlagK;//��������
	Circle(
			int texIdIn,
			float m_r,//�뾶
			int m_n,//Բ���и�ķ���    30
			btVector3* m_pos,
			int m_index
			);//xoz���ϵ�Բ   �ǳ��ϵ�Բ
	Circle(
			int texIdIn,
			float m_r,//�뾶
			int m_n,//Բ���и�ķ���    30
			float h//Բ�ĸ߶�    0
			);//xoz���ϵ�Բ   �ǳ��ϵ�Բ
    void initShader();
    void drawSelf(int m_texId);
    void refresh();
};

#endif
