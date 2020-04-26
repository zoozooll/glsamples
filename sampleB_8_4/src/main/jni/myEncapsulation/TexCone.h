#ifndef _TexCone_H_
#define _TexCone_H_

#include "TexBody.h"
#include "MatrixState.h"
#include "../Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "../Bullet/LinearMath/btVector3.h"

#include "geometry/ConeSide.h"
#include "geometry/Circle.h"

class TexCone : public TexBody
{
	btRigidBody* body;
	btDynamicsWorld *m_dynamicsWorld;//�����������

	float radius;//�뾶
	float height;//�߶�

	int sideTexId;//Բ׶�������id
	int circleTexId;//Բ׶��Բ����id

	Circle* bottomCircle;//��Բָ��
	ConeSide* coneSide;//Բ׶����

public:
	TexCone(
		btScalar mass,//����
		float radiusIn,//�뾶
		float heightIn,//�߶�
		btVector3 pos,//λ��
		btScalar restitutionIn,//�ָ�ϵ��
		btScalar frictionIn,//Ħ��ϵ��
		btDynamicsWorld *dynamicsWorld,//��������ָ��
		int sliderTexIdIn,//Բ׶�������id
		int circleTexIdIn,//Բ׶��Բ����id
		int n//���зֵķ���  n
		);
	btRigidBody* getBody();
	void drawSelf();
};

#endif
