#ifndef _VECTOR4F_H_
#define _VECTOR4F_H_

//表示具有四个分量的数据（如齐次坐标、四元数等）
class Vector4f {
private:

public:
	float* coording;//存放四个分量值的数组

	Vector4f();

	Vector4f(float x1, float x2, float x3, float x4);

	~Vector4f();

	//设置旋转数据，入口参数为欧拉角的三个分量
	//转换为四元数的四个分量XYZW
	void setFromEulerAngleToQuaternion(float yaw, float pitch, float roll);

	//实施v1与v2两个四元数按照alpha与1-alpha的比例融合插值
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
