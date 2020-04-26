#include "Vector3f.h"

Vector3f::Vector3f() {
	coord=new float[3];
	this->coord[0] = 0;
	this->coord[1] = 0;
	this->coord[2] = 0;
}

Vector3f::Vector3f(float x, float y, float z) {
	coord=new float[3];
	this->coord[0] = x;
	this->coord[1] = y;
	this->coord[2] = z;
}

Vector3f::~Vector3f()
{
	delete coord;
}

//�����������ձ������
void Vector3f::interpolate(Vector3f* v1, Vector3f* v2, float alpha) {
	this->setX((1 - alpha) * v1->getX() + alpha * v2->getX());
	this->setY((1 - alpha) * v1->getY() + alpha * v2->getY());
	this->setZ((1 - alpha) * v1->getZ() + alpha * v2->getZ());
}

float* Vector3f:: getVector3fArray()
{
		return coord;
}

void Vector3f::setX(float x) {
	this->coord[0] = x;
}

void Vector3f::setY(float y) {
	this->coord[1] = y;
}

void Vector3f::setZ(float z) {
	this->coord[2] = z;
}

float Vector3f::getX()
{
	return this->coord[0];
}

float Vector3f::getY()
{
	return this->coord[1];
}

float Vector3f::getZ()
{
	return this->coord[2];
}
