#include "TexCone.h"
#include "ShaderUtil.h"
#include "Bullet/BulletCollision/CollisionShapes/btConeShape.h"

TexCone::TexCone(
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
	)
{
	bool isDynamic = (mass != 0);//�����Ƿ�����˶�

	btConeShape* coneShape = new btConeShape(radiusIn,heightIn);

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
	this->sideTexId = sliderTexIdIn;//Բ׶�������id
	this->circleTexId = circleTexIdIn;//Բ׶��Բ����id

	bottomCircle = new Circle(
			circleTexId,
			radiusIn,//�뾶
			n,//Բ���и�ķ���   30
			0//Բ�ĸ߶�  0
			);//xoz���ϵ�Բ   �ǳ��ϵ�Բ
	coneSide = new ConeSide(
			sideTexId,
			radiusIn,//�뾶
			n,//�зֵķ���
			heightIn//Բ���ĸ߶�
			);//��Ϊy���Բ�����
}

void TexCone::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;									//��ȡ������ӵı任��Ϣ����
	trans = body->getWorldTransform();				//��ȡ����ı任��Ϣ����
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//����ǰ�ľ������ø��任��Ϣ����

	if(body->isActive())
	{
		coneSide->mFlagK = 0;
		bottomCircle->mFlagK = 0;
	}
	else
	{
		coneSide->mFlagK = 1;
		bottomCircle->mFlagK = 1;
	}

	//�����
	MatrixState::pushMatrix();//�����ֳ�
	coneSide->drawSelf(sideTexId);
	MatrixState::popMatrix();//�ָ��ֳ�

	//����Բ
	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, -height/2, 0);//ִ��ƽ��
	MatrixState::rotate(180,1,0,0);
	bottomCircle->drawSelf(circleTexId);
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}

btRigidBody* TexCone::getBody()
{
	return body;
}
