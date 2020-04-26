#ifndef _MAT4_H_
#define _MAT4_H_

#include "Vector3f.h"
#include "Vector4f.h"

class Mat4{
public:
	float* matrix;//用于存储矩阵元素的数组
public:
	Mat4();
	~Mat4();
	//将矩阵设置为单位矩阵
	void loadIdentity();
	//设置平移矩阵参数
	void setTranslation(Vector3f* v);
	//设置旋转矩阵参数，入口参数为欧拉角三个分量组成的三维向量
	//实质上是将欧拉角形式的旋转描述转化为矩阵形式的旋转描述
	void genRotationFromEulerAngle(Vector3f* angles);////////////////////////////////////////
	//两个矩阵相乘
	Mat4* mul(Mat4* m1, Mat4* m2);
	//把一个矩阵的元素值依次拷贝到本矩阵中
	void copyFrom(Mat4* m);
	//产生旋转矩阵，实际为将四元数形式的旋转描述
	//转化为以矩阵形式描述的旋转
	void genRotateFromQuaternion(Vector4f* v);
	//将顶点通过此矩阵进行逆变换得到新的位置
	Vector3f* invTransformAndRotate(Vector3f* point);
	//将顶点通过此矩阵进行变换得到新的位置
	Vector3f* transform(Vector3f* point);
	//设置沿xyz轴移动
	void translate(float x, float y, float z);
	//设置旋转
	void rotate(float angle, float x, float y, float z);
	//设置缩放
	void scale(float x, float y, float z);

	static void print_matrix(Mat4 m);

};

#endif
