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

	btConeShape* coneShape = new btConeShape(radiusIn,heightIn);//����Բ׶��״

	btVector3 localInertia = btVector3(0, 0, 0);//��������
	if(isDynamic)
	{
		coneShape->calculateLocalInertia(mass, localInertia);//�������
	}
	btTransform transform = btTransform();//��������ĳ�ʼ�任����
	transform.setIdentity();//�任�����ʼ��
	transform.getOrigin().setValue(pos.x(),pos.y(),pos.z());//���ñ任�����λ��
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);//���������˶�״̬����
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, coneShape, localInertia);//����������Ϣ����
	rbInfo.m_restitution = restitutionIn;//���÷���ϵ��
	rbInfo.m_friction = frictionIn;//����Ħ��ϵ��
	body = new btRigidBody(rbInfo);//��������
	dynamicsWorld->addRigidBody(body);//��������ӽ���������
	this->radius = radiusIn;//��¼��Բ�뾶
	this->height = heightIn;//��¼Բ׶�߶�
	this->sideTexId = sliderTexIdIn;//��¼Բ׶�������id
	this->circleTexId = circleTexIdIn;//��¼Բ׶��Բ����id
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

	if(body->isActive())//�жϸ��嵱ǰ�Ƿ�ɶ�
	{
		coneSide->mFlagK = 0;//����Բ׶������������
		bottomCircle->mFlagK = 0;//���õ�Բ��������
	}
	else
	{
		coneSide->mFlagK = 1;//����Բ׶������������
		bottomCircle->mFlagK = 1;//���õ�Բ��������
	}

	//�����
	MatrixState::pushMatrix();//�����ֳ�
	coneSide->drawSelf(sideTexId);//����Բ׶����
	MatrixState::popMatrix();//�ָ��ֳ�

	//����Բ
	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, -height/2, 0);//ִ��ƽ��
	MatrixState::rotate(180,1,0,0);//ִ����ת
	bottomCircle->drawSelf(circleTexId);//���Ƶ�Բ
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}

btRigidBody* TexCone::getBody()
{
	return body;//���ظ���ָ��
}
