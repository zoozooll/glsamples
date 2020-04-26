#ifndef _VECTOR3F_H_
#define _VECTOR3F_H_

//表示具有三个分量的复合数的类（如顶点位置、法向量等）
class Vector3f {
public:
	float* coord;//XYZ分量数据数组
public:
	Vector3f();
	Vector3f(float x, float y, float z);
	~Vector3f();
	void interpolate(Vector3f* v1, Vector3f* v2, float alpha);//两个三维向量按照比例混合
	float* getVector3fArray();
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	float getX();
	float getY();
	float getZ();
};

#endif
