#ifndef _TexCapsule_H_
#define _TexCapsule_H_

#include "TexBody.h"
#include "MatrixState.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/LinearMath/btVector3.h"

#include "geometry/Ball.h"
#include "geometry/CylinderSide.h"

class TexCapsule : public TexBody
{
	btRigidBody* body;

	float radius;//�뾶
	float height;//�߶�

	int capuleSideTexId;//���Ҳ������id
	int buttomBallTexId;//�ײ�Բ����id
	int topBallTexId;//����Բ����id

public:
	CylinderSide* capuleSide;//���ҵĲ���
	Ball* bottomBall;//�ײ�Բ

public:
	TexCapsule(
		btScalar mass,//����
		float radiusIn,//�뾶
		float heightIn,//�߶�
		btVector3 pos,//λ��
		btScalar restitutionIn,//�ָ�ϵ��
		btScalar frictionIn,//Ħ��ϵ��
		btDynamicsWorld *dynamicsWorld,//��������ָ��
		int capuleSideTexIdIn,//���Ҳ������id
		int buttomBallTexIdIn,//�ײ�Բ����id
		int topBallTexIdIn,//����Բ����id
		int n//���зֵķ���  n
		);

	TexCapsule(
			float radiusIn,//�뾶
			float heightIn,//�߶�
			int capuleSideTexIdIn,//���Ҳ������id
			int buttomBallTexIdIn,//�ײ�Բ����id
			int topBallTexIdIn,//����Բ����id
			int n//���зֵķ���  n
			);
	btRigidBody* getBody();
	void drawSelfNoBody();
	void drawSelf();
};

#endif
