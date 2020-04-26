#ifndef _AnyShapeByVectices_H_
#define _AnyShapeByVectices_H_

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../Bullet/LinearMath/btVector3.h"

class AnyShapeByVectices
{

	GLuint mProgram;//�Զ�����Ⱦ���߳���id
	GLuint muMVPMatrixHandle;//�ܱ任����
	GLuint muMMatrixHandle;//λ�á���ת�任����
	GLuint muTexHandle;//�����������
	GLuint muLightLocationHandle;//��Դλ������
	GLuint muCameraHandle;//�����λ������

	GLuint maPositionHandle;//����λ������
	GLuint maTexCoorHandle;//����������������
	GLuint maNormalHandle;//���㷨��������

	const GLvoid* mVertexBuffer;//�����������ݻ���
	const GLvoid* mNormalBuffer;//���㷨�������ݻ���
	const GLvoid* mTextureBuffer;//�����������ݻ���


	int m_vCount;//�������� ���ظ�
	int m_vCount_noRepeat;//�������� ���ظ�
	float* m_normals;
	float* m_vertices;
	float* m_vertices_noRepeat;
	btVector3* m_vertices_bt;
	int* m_indices;//��������
	float m_lightFlag;// Ϊ1.0f��ʾ˳ʱ��     Ϊ-1.0f��ʾ��ʱ��  	 һ��Ϊ-1.0f

	bool update;//������Ķ��㲻�Ϸ����任ʱ���Ƿ���и���

public:

	//������û�ж�������ֵ�����    ֻ�ж������飨���ظ�ֵ��
	AnyShapeByVectices(
			float* vertices_fl,//����*3����	���ظ�ֵ
			int numsVerticesBt,//����*3���������ظ�ֵ��
			float* m_texs,
			int m_nums_tex,
			bool update,//��״��Ҫ�����任ʱ��Ϊtrue����״����Ҫ�����任ʱ��Ϊfalse
			float lightFlag// Ϊ1��ʾ˳ʱ��     Ϊ-1��ʾ��ʱ��
			);//������

	AnyShapeByVectices(
			float* vertices_fl,//��������	���ظ�ֵ
			int numsVerticesBt,//�������������ظ�ֵ��
			int* indices,//��������
			int numsIndices,//��������
			float* m_texs,
			int m_nums_tex,
			bool update,//��״��Ҫ�����任ʱ��Ϊtrue����״����Ҫ�����任ʱ��Ϊfalse
			float lightFlag// Ϊ1��ʾ˳ʱ��     Ϊ-1��ʾ��ʱ��
			);//������

    void initShader();
    void drawSelf(int texId);
    void initIndices();

    float* method(
    		btVector3* vertices,//��������(���ظ�ֵ��)
    		int nums_vertices,//��������ĳ���
    		int* index_vertices,//�����������������
    		int nums_indexVertices//�����������������ĳ���	ע�⣺Ӧ����nums_vertices��ͬ
    		);

	void initVNData();

	float* btVector3ToFloat(btVector3* vertex, int nums);//�������飬�������� �������鳤�� nums*3

	btVector3* floatToBtVector3(float* vertex, int nums);//�������飬��������  �������鳤�� nums/3
};

#endif
