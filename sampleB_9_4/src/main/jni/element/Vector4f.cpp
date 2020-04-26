#include "Vector4f.h"
#include "math.h"

Vector4f::Vector4f() {
	coording=new float[4];
	this->coording[0] = 0;
	this->coording[1] = 0;
	this->coording[2] = 0;
	this->coording[3] = 0;
}

Vector4f::Vector4f(float x1, float x2, float x3, float x4)
{
	coording=new float[4];
	this->coording[0] = x1;
	this->coording[1] = x2;
	this->coording[2] = x3;
	this->coording[3] = x4;
}

Vector4f::~Vector4f()
{
	delete coording;
}

//设置旋转数据，入口参数为欧拉角的三个分量
//转换为四元数的四个分量XYZW
void Vector4f::setFromEulerAngleToQuaternion(float yaw, float pitch, float roll) {
	float angle = 0.0f;
	float sr, sp, sy, cr, cp, cy;
	angle = yaw * 0.5f;
	sr = (float) sin(angle);
	cr = (float) cos(angle);
	angle = pitch * 0.5f;
	sp = (float) sin(angle);
	cp = (float) cos(angle);
	angle = roll * 0.5f;
	sy = (float) sin(angle);
	cy = (float) cos(angle);

	this->setX(sr * cp * cy - cr * sp * sy); // X
	this->setY(cr * sp * cy + sr * cp * sy); // Y
	this->setZ(cr * cp * sy - sr * sp * cy); // Z
	this->setW(cr * cp * cy + sr * sp * sy); // W

}

//实施v1与v2两个四元数按照alpha与1-alpha的比例融合插值
void Vector4f::interpolate(Vector4f* v1, Vector4f* v2, float alpha) {
	double dot = 0, s1, s2, om, sinom;
	//求两个四元数的点积
	for (int i = 0; i < 4; i++) {
		dot += v2->coording[i] * v1->coording[i];
	}
	Vector4f v0;
	//若点积值小于0则将v1置反，同时将点积值也置反
	if (dot < 0) {
		v0.coording[0] = -v1->coording[0];
		v0.coording[1] = -v1->coording[1];
		v0.coording[2] = -v1->coording[2];
		v0.coording[3] = -v1->coording[3];

		dot = -dot;
	} else { //否则直接采用v1的值
		v0.coording[0] = v1->coording[0];
		v0.coording[1] = v1->coording[1];
		v0.coording[2] = v1->coording[2];
		v0.coording[3] = v1->coording[3];
	}

	if (dot > 0.999999) { //若点积值接近1，则说明两个四元数表示的旋转非常接近       直接线性插值即可
		s1 = 1.0 - alpha;
		s2 = alpha;
	} else { //否则按照思维空间中的圆弧来计算插值系数
		om = acos(dot);
		sinom = sin(om);
		s1 = sin((1.0 - alpha) * om) / sinom;
		s2 = sin(alpha * om) / sinom;
	}

	//通过插值系数s1、s2计算v1、v2两个四元比例数插值后
	//的新四元数的四个分量
	float tw=(float) (s1 * v0.getW() + s2 * v2->getW());
	float tx=(float) (s1 * v0.getX() + s2 * v2->getX());
	float ty=(float) (s1 * v0.getY() + s2 * v2->getY());
	float tz=(float) (s1 * v0.getZ() + s2 * v2->getZ());
	this->setW(tw);
	this->setX(tx);
	this->setY(ty);
	this->setZ(tz);
}

void Vector4f::setX(float x) {
	this->coording[0] = x;
}

void Vector4f::setY(float y) {
	this->coording[1] = y;
}

void Vector4f::setZ(float z) {
	this->coording[2] = z;
}

void Vector4f::setW(float w) {
	this->coording[3] = w;
}

float Vector4f::getX() {
	return this->coording[0];
}

float Vector4f::getY() {
	return this->coording[1];
}

float Vector4f::getZ() {
	return this->coording[2];
}

float Vector4f::getW() {
	return this->coording[3];
}
