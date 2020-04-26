#ifndef _MAT4_H_
#define _MAT4_H_

#include "Vector3f.h"
#include "Vector4f.h"

class Mat4{
public:
	float* matrix;//���ڴ洢����Ԫ�ص�����
public:
	Mat4();
	~Mat4();
	//����������Ϊ��λ����
	void loadIdentity();
	//����ƽ�ƾ������
	void setTranslation(Vector3f* v);
	//������ת�����������ڲ���Ϊŷ��������������ɵ���ά����
	//ʵ�����ǽ�ŷ������ʽ����ת����ת��Ϊ������ʽ����ת����
	void genRotationFromEulerAngle(Vector3f* angles);////////////////////////////////////////
	//�����������
	Mat4* mul(Mat4* m1, Mat4* m2);
	//��һ�������Ԫ��ֵ���ο�������������
	void copyFrom(Mat4* m);
	//������ת����ʵ��Ϊ����Ԫ����ʽ����ת����
	//ת��Ϊ�Ծ�����ʽ��������ת
	void genRotateFromQuaternion(Vector4f* v);
	//������ͨ���˾��������任�õ��µ�λ��
	Vector3f* invTransformAndRotate(Vector3f* point);
	//������ͨ���˾�����б任�õ��µ�λ��
	Vector3f* transform(Vector3f* point);
	//������xyz���ƶ�
	void translate(float x, float y, float z);
	//������ת
	void rotate(float angle, float x, float y, float z);
	//��������
	void scale(float x, float y, float z);

	static void print_matrix(Mat4 m);

};

#endif
