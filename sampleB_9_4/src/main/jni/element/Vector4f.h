#ifndef _VECTOR4F_H_
#define _VECTOR4F_H_

//��ʾ�����ĸ����������ݣ���������ꡢ��Ԫ���ȣ�
class Vector4f {
private:

public:
	float* coording;//����ĸ�����ֵ������

	Vector4f();

	Vector4f(float x1, float x2, float x3, float x4);

	~Vector4f();

	//������ת���ݣ���ڲ���Ϊŷ���ǵ���������
	//ת��Ϊ��Ԫ�����ĸ�����XYZW
	void setFromEulerAngleToQuaternion(float yaw, float pitch, float roll);

	//ʵʩv1��v2������Ԫ������alpha��1-alpha�ı����ںϲ�ֵ
	void interpolate(Vector4f* v1, Vector4f* v2, float alpha);

	void setX(float x);

	void setY(float y);

	void setZ(float z);

	void setW(float w);

	float getX();

	float getY();

	float getZ();

	float getW();
};

#endif
