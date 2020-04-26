#ifndef _MyCompoundShape_H_
#define _MyCompoundShape_H_

#include <string.h>
#include <jni.h>
#include <vector>
#include "../Bullet/LinearMath/btAlignedObjectArray.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/LinearMath/btVector3.h"
#include "../Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "myEncapsulation/MatrixState.h"
#include "../myEncapsulation/TexCuboid.h"
#include "../myEncapsulation/TexBody.h"
#include "../myEncapsulation/TexPlane.h"
#include "../myEncapsulation/TexBall.h"
#include "../myEncapsulation/TexCapsule.h"
#include "../myEncapsulation/TexCone.h"
#include "../myEncapsulation/TexCylinderShape.h"


class MyCompoundShape : public TexBody
{
	btRigidBody* body;//����ָ��
	TexCapsule* tc;//��������ָ��
	TexCylinderShape* tcs;//Բ��������ָ��
public:
	MyCompoundShape(
			btDynamicsWorld* dynamicsWorld,
			btScalar mass,
			btVector3 pos,
			float restitutionIn,
			float frictionIn,
			int texId_capusleSide,//���Ҳ������id
			int texId_buttomBall,//�ײ�Բ����id
			int texId_topBall,//����Բ����id
			int texId_bottomCircle,//�ײ�Բ�ε�����id
			int texId_topCircle,//����Բ�ε�����id
			int texId_cylinderSide//Բ�����������id
			);
    void drawSelf();
    btRigidBody* getBody();
};

#endif
