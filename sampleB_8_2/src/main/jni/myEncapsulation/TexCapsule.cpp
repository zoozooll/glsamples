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

	btCapsuleShape* coneShape = new btCapsuleShape(radiusIn,heightIn);//����������״

	btVector3 localInertia = btVector3(0, 0, 0);//��������
	if(isDynamic)
	{
		coneShape->calculateLocalInertia(mass, localInertia);//�������
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

	this->radius = radiusIn;//��¼�뾶
	this->height = heightIn;//��¼�߶�
	this->capuleSideTexId = capuleSideTexIdIn;//���Ҳ������id
	this->topBallTexId = topBallTexIdIn;//�ײ�������id
	this->buttomBallTexId = buttomBallTexIdIn;//����������id

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

	if(body->isActive())//�жϸ����Ƿ�Ϊ��̬
	{
		capuleSide->mFlagK = 0;//���ò�����������
		bottomBall->mFlagK = 0;//��������������
	}
	else
	{
		capuleSide->mFlagK = 1;//���ò�����������
		bottomBall->mFlagK = 1;//��������������
	}

	//�����
	MatrixState::pushMatrix();//�����ֳ�
	capuleSide->drawSelf(capuleSideTexId);//���ƽ��Ҳ���
	MatrixState::popMatrix();//�ָ��ֳ�

	//
	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, -height/2, 0);//ִ��ƽ��
	bottomBall->drawSelf(topBallTexId);//�����²����
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, height/2, 0);//ִ��ƽ��
	bottomBall->drawSelf(topBallTexId);//�����ϲ����
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}

btRigidBody* TexCapsule::getBody()
{
	return body;//���ظ���ָ��
}
