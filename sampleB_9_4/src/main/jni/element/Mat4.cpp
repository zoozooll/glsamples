#include "Mat4.h"
#include "../myEncapsulation/Matrix.h"

#include <math.h>
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

Mat4::Mat4(){
	this->matrix = new float[16];
}

Mat4::~Mat4()
{
	delete matrix;
}

//将矩阵设置为单位矩阵
void Mat4::loadIdentity()
{
	this->matrix = new float[16];
	Matrix::setIdentityM(matrix, 0);
}

//设置平移矩阵参数
void Mat4::setTranslation(Vector3f* v)
{
	this->matrix[12] = v->getX();
	this->matrix[13] = v->getY();
	this->matrix[14] = v->getZ();
}

//设置旋转矩阵参数，入口参数为欧拉角三个分量组成的三维向量
//实质上是将欧拉角形式的旋转描述转化为矩阵形式的旋转描述
void Mat4::genRotationFromEulerAngle(Vector3f* angles)
{
	float yaw=angles->getX();
	float pitch=angles->getY();
	float roll=angles->getZ();

	double cr = cos(yaw);
    double sr = sin(yaw);
    double cp = cos(pitch);
    double sp = sin(pitch);
    double cy = cos(roll);
    double sy = sin(roll);
    double srsp = sr * sp;
    double crsp = cr * sp;

    float* mTemp=new float[16];
    Matrix::setIdentityM(mTemp,0);

    mTemp[0] = (float) (cp * cy);
    mTemp[1] = (float) (cp * sy);
    mTemp[2] = (float) (-sp);

    mTemp[4] = (float) (srsp * cy - cr * sy);
    mTemp[5] = (float) (srsp * sy + cr * cy);
    mTemp[6] = (float) (sr * cp);

    mTemp[8] = (float) (crsp * cy + sr * sy);
    mTemp[9] = (float) (crsp * sy - sr * cy);
    mTemp[10] = (float) (cr * cp);

    float* mTempR=new float[16];
    Matrix::multiplyMM(mTempR, 0, matrix, 0, mTemp, 0);
    this->matrix=mTempR;
}

//两个矩阵相乘
Mat4* Mat4::mul(Mat4* m1, Mat4* m2)
{
	float* mData = new float[16];
	Matrix::multiplyMM(mData, 0, m1->matrix, 0, m2->matrix, 0);
	this->matrix = mData;
	return this;
}

//把一个矩阵的元素值依次拷贝到本矩阵中
void Mat4::copyFrom(Mat4* m)
{
	for(int i = 0; i < 16; i++)
	{
		this->matrix[i] = m->matrix[i];
	}
}

//产生旋转矩阵，实际为将四元数形式的旋转描述
//转化为以矩阵形式描述的旋转
void Mat4::genRotateFromQuaternion(Vector4f* v)
{
	float x=v->getX();
	float y=v->getY();
	float z=v->getZ();
	float w=v->getW();
	this->matrix[0] = (1.0f - 2.0f * y * y - 2.0f * z * z);
	this->matrix[1] = (2.0f * (x * y + w * z));
	this->matrix[2] = (2.0f * (x * z - w * y));

	this->matrix[4] = (2.0f * (x * y - w * z));
	this->matrix[5] = (1.0f - 2.0f * x * x - 2.0f * z * z);
	this->matrix[6] = (2.0f * (y * z + w * x));

	this->matrix[8] = (2.0f * (x * z + w * y));
	this->matrix[9] = (2.0f * (y * z - w * x));
	this->matrix[10] = (1.0f - 2.0f * x * x - 2.0f * y * y);

	this->matrix[12] = (float) 0.0;
	this->matrix[13] = (float) 0.0;
	this->matrix[14] = (float) 0.0;

	this->matrix[3] = (float) 0.0;
	this->matrix[7] = (float) 0.0;
	this->matrix[11] = (float) 0.0;
	this->matrix[15] = (float) 1.0;
}

//将顶点通过此矩阵进行逆变换得到新的位置
Vector3f* Mat4::invTransformAndRotate(Vector3f* point)
{
//	LOGI("==Mat4==invTransformAndRotate===start===");
	Vector3f* v=new Vector3f();
	//逆平移部分
	float x = point->getX() - this->matrix[12];
	float y = point->getY() - this->matrix[13];
	float z = point->getZ() - this->matrix[14];
//	LOGI("==Mat4==invTransformAndRotate===11111111111111===%f %f %f",x,y,z);
	//逆旋转部分
	float tx=this->matrix[0] * x + this->matrix[1] * y + this->matrix[2]* z;
	float ty=this->matrix[4] * x + this->matrix[5] * y + this->matrix[6] * z;
	float tz=this->matrix[8] * x + this->matrix[9] * y + this->matrix[10]* z;
//	LOGI("==Mat4==invTransformAndRotate===22222222222222===%f %f %f",tx,ty,tz);
	v->setX(tx);
	v->setY(ty);
	v->setZ(tz);
//	LOGI("==Mat4==invTransformAndRotate===end===");
	return v;
}

//将顶点通过此矩阵进行变换得到新的位置
Vector3f* Mat4::transform(Vector3f* point)
{
//	LOGI("==Mat4==transform===start===");
	Vector3f* v=new Vector3f();
	v->setX(
			this->matrix[0] * point->getX()
			+ this->matrix[4] * point->getY()
			+ this->matrix[8] * point->getZ()
			+ this->matrix[12]);
	v->setY(
			this->matrix[1] * point->getX()
			+ this->matrix[5] * point->getY()
			+ this->matrix[9] * point->getZ()
			+ this->matrix[13]);
	v->setZ(
			this->matrix[2] * point->getX()
			+ this->matrix[6] * point->getY()
			+ this->matrix[10] * point->getZ()
			+ this->matrix[14]);
//	LOGI("==Mat4==transform===end===");
	return v;
}

//设置沿xyz轴移动
void Mat4::translate(float x, float y, float z)
{
	Matrix::translateM(matrix, 0, x, y, z);
}

//设置旋转
void Mat4::rotate(float angle, float x, float y, float z)
{
	Matrix::rotateM(matrix, 0, angle, x, y, z);
}

//设置缩放
void Mat4::scale(float x, float y, float z)
{
	Matrix::scaleM(this->matrix, 0, x, y, z);
}

void Mat4::print_matrix(Mat4 m)
{
	for(int i = 0; i < 4; i++)
	{
//		System.out.println(m.matrix[i*4] + " " + m.matrix[i*4 + 1] + " " + m.matrix[i*4 + 2] + " " + m.matrix[i*4 + 3]);
		LOGI("%f,%f,%f,%f",m.matrix[i*4],m.matrix[i*4+1],m.matrix[i*4+2],m.matrix[i*4+3]);
	}
}
