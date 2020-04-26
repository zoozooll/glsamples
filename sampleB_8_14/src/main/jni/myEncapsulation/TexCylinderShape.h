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

	float halfSize;//������İ�߳�
	btRigidBody *body;//��Ӧ�ĸ������

	int texId_bottomCircle;//�ײ�Բ�ε�����id
	int texId_topCircle;//����Բ�ε�����id
	int texId_cylinderSide;//Բ�����������id

	btDynamicsWorld *m_dynamicsWorld;//�����������
	float offset_topCircle;//���ƶ�Բ�����λ����

public:
	Circle* bottomCircle;//��Բ
	CylinderSide* cylinderSide;//����

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
	TexCylinderShape(
			int texId_bottomCircleIn,//�ײ�Բ�ε�����id
			int texId_topCircleIn,//����Բ�ε�����id
			int texId_cylinderSideIn,//Բ�����������id
			int n,//�зַ���
			btVector3 halfExtents//������
			);//��ʼԲ������ƽ����y�ᣬԲ����ƽ��xoz�����ϵ�
	void drawSelf();
	void drawSelfNoBody();
	btRigidBody* getBody();
};
#endif
