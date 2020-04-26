#include "TexCapsule.h"
#include "ShaderUtil.h"
#include "Bullet/BulletCollision/CollisionShapes/btCapsuleShape.h"

TexCapsule::TexCapsule(
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
		)
{
	bool isDynamic = (mass != 0);//�����Ƿ�����˶�

	btCapsuleShape* coneShape = new btCapsuleShape(radiusIn,heightIn);

	btVector3 localInertia = btVector3(0, 0, 0);//��������
	if(isDynamic)
	{
		coneShape->calculateLocalInertia(mass, localInertia);
	}
	btTransform transform = btTransform();//��������ĳ�ʼ�任����
	transform.setIdentity();//�任�����ʼ��
	transform.getOrigin().setValue(pos.x(),pos.y(),pos.z());//���ñ任�����λ��
	//���������˶�״̬����
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	//����������Ϣ����
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, coneShape, localInertia);
	rbInfo.m_restitution = restitutionIn;//���÷���ϵ��
	rbInfo.m_friction = frictionIn;//����Ħ��ϵ��
	body = new btRigidBody(rbInfo);//��������
	dynamicsWorld->addRigidBody(body);//��������ӽ���������

	this->radius = radiusIn;//�뾶
	this->height = heightIn;//�߶�
	this->capuleSideTexId = capuleSideTexIdIn;//���Ҳ������id
	this->topBallTexId = topBallTexIdIn;//�ײ�Բ����id
	this->buttomBallTexId = buttomBallTexIdIn;//����Բ����id

	capuleSide = new CylinderSide(
			capuleSideTexId,
			radius,//�뾶
			n,//�зֵķ���
			height//Բ���ĸ߶�
			);//��Ϊy���Բ�����
	bottomBall = new Ball(
			topBallTexId,
			radius//�뾶
			);
}

TexCapsule::TexCapsule(
		float radiusIn,//�뾶
		float heightIn,//�߶�
		int capuleSideTexIdIn,//���Ҳ������id
		int buttomBallTexIdIn,//�ײ�Բ����id
		int topBallTexIdIn,//����Բ����id
		int n//���зֵķ���  n
		)
{
	this->radius = radiusIn;//�뾶
	this->height = heightIn;//�߶�
	this->capuleSideTexId = capuleSideTexIdIn;//���Ҳ������id
	this->topBallTexId = topBallTexIdIn;//�ײ�Բ����id
	this->buttomBallTexId = buttomBallTexIdIn;//����Բ����id

	capuleSide = new CylinderSide(
			capuleSideTexId,
			radius,//�뾶
			n,//�зֵķ���
			height//Բ���ĸ߶�
			);//��Ϊy���Բ�����
	bottomBall = new Ball(
			topBallTexId,
			radius//�뾶
			);
}

void TexCapsule::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;									//��ȡ������ӵı任��Ϣ����
	trans = body->getWorldTransform();				//��ȡ����ı任��Ϣ����
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//����ǰ�ľ������ø��任��Ϣ����

	if(body->isActive())
	{
		capuleSide->mFlagK = 0;
		bottomBall->mFlagK = 0;
	}
	else
	{
		capuleSide->mFlagK = 1;
		bottomBall->mFlagK = 1;
	}

	//�����
	MatrixState::pushMatrix();//�����ֳ�
	capuleSide->drawSelf(capuleSideTexId);
	MatrixState::popMatrix();//�ָ��ֳ�

	//
	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, -height/2, 0);//ִ��ƽ��
	bottomBall->drawSelf(topBallTexId);
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, height/2, 0);//ִ��ƽ��
	bottomBall->drawSelf(topBallTexId);
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}

void TexCapsule::drawSelfNoBody()
{
	MatrixState::pushMatrix();//�����ֳ�

	//�����
	MatrixState::pushMatrix();//�����ֳ�
	capuleSide->drawSelf(capuleSideTexId);
	MatrixState::popMatrix();//�ָ��ֳ�

	//
	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, -height/2, 0);//ִ��ƽ��
	bottomBall->drawSelf(topBallTexId);
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, height/2, 0);//ִ��ƽ��
	bottomBall->drawSelf(topBallTexId);
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}

btRigidBody* TexCapsule::getBody()
{
	return body;
}
