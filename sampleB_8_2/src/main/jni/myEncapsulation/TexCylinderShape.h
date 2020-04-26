#ifndef __TexCylinderShape_h
#define __TexCylinderShape_h
#include "TexBody.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/LinearMath/btVector3.h"
#include "../Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "MatrixState.h"

#include "geometry/CylinderSide.h"
#include "geometry/Circle.h"

class TexCylinderShape:public TexBody{

	btRigidBody *body;//��Ӧ�ĸ������

	int texId_bottomCircle;//�ײ�Բ�ε�����id
	int texId_topCircle;//����Բ�ε�����id
	int texId_cylinderSide;//Բ�����������id

	Circle* bottomCircle;//��Բ
	CylinderSide* cylinderSide;//����
	float offset_topCircle;//���ƶ�Բ�����λ����
public:
	TexCylinderShape(
			int texId_bottomCircle,//�ײ�Բ�ε�����id
			int texId_topCircle,//����Բ�ε�����id
			int texId_cylinderSide,//Բ�����������id
			int n,//�зַ���  30
			btVector3 halfExtents,//������
			btDynamicsWorld *dynamicsWorld,//��������ָ��
			btScalar mass,//����
			btVector3 pos,//λ��
			btScalar restitutionIn,//�ָ�ϵ��
			btScalar frictionIn//Ħ��ϵ��
			);//��ʼԲ������ƽ����y�ᣬԲ����ƽ��xoz�����ϵ�
	void drawSelf();//�����������ķ���
	btRigidBody* getBody();//��ȡ�������ķ���
};
#endif
