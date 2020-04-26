#include "TexCylinderShape.h"

TexCylinderShape::TexCylinderShape(
		int texId_bottomCircleIn,//�ײ�Բ�ε�����id
		int texId_topCircleIn,//����Բ�ε�����id
		int texId_cylinderSideIn,//Բ�����������id
		int n,//�зַ���
		btVector3 halfExtents,//������
		btDynamicsWorld *dynamicsWorld,
		btScalar mass,
		btVector3 pos,
		btScalar restitutionIn,
		btScalar frictionIn)
{
	bool isDynamic = (mass != 0);//�����Ƿ�����˶�
	btVector3 localInertia = btVector3(0, 0, 0);//��������
	btCollisionShape* colShape = new btCylinderShape(halfExtents);
	if(isDynamic) //�����������˶�
	{
		colShape->calculateLocalInertia(mass, localInertia);//�������
	}
	btTransform startTransform = btTransform();//��������ĳ�ʼ�任����
	startTransform.setIdentity();//�任��ʼ��
	startTransform.getOrigin().setValue(pos.x(), pos.y(), pos.z());//���ó�ʼ��λ��
	//����������˶�״̬����
	btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
	//����������Ϣ����
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
	rbInfo.m_restitution = restitutionIn;//���÷���ϵ��
	rbInfo.m_friction = frictionIn;//����Ħ��ϵ��
	body = new btRigidBody(rbInfo);//��������

	dynamicsWorld->addRigidBody(body);//��������ӽ���������

	this->m_dynamicsWorld = dynamicsWorld;

	bottomCircle = new Circle(
			texId_bottomCircleIn,
			halfExtents.x(),//�뾶
			n,//Բ���и�ķ���
			0//-halfExtents.y()//Բ�ĸ߶�
			);//��Բ   Բ�ǳ��ϵ�
	offset_topCircle = halfExtents.y();
	cylinderSide = new CylinderSide(
			texId_cylinderSideIn,
			halfExtents.x(),//�뾶
			n,//����������
			halfExtents.y()*2//Բ���ĸ߶�
			);

	this->texId_bottomCircle = texId_bottomCircleIn;
	this->texId_topCircle = texId_topCircleIn;
	this->texId_cylinderSide = texId_cylinderSideIn;
}

TexCylinderShape::TexCylinderShape(
		int texId_bottomCircleIn,//�ײ�Բ�ε�����id
		int texId_topCircleIn,//����Բ�ε�����id
		int texId_cylinderSideIn,//Բ�����������id
		int n,//�зַ���
		btVector3 halfExtents//������
		)
{

	bottomCircle = new Circle(
			texId_bottomCircleIn,
			halfExtents.x(),//�뾶
			n,//Բ���и�ķ���
			0//-halfExtents.y()//Բ�ĸ߶�
			);//��Բ   Բ�ǳ��ϵ�
	offset_topCircle = halfExtents.y();
	cylinderSide = new CylinderSide(
			texId_cylinderSideIn,
			halfExtents.x(),//�뾶
			n,//����������
			halfExtents.y()*2//Բ���ĸ߶�
			);

	this->texId_bottomCircle = texId_bottomCircleIn;
	this->texId_topCircle = texId_topCircleIn;
	this->texId_cylinderSide = texId_cylinderSideIn;
}

void TexCylinderShape::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;									//��ȡ������ӵı任��Ϣ����
	trans = body->getWorldTransform();				//��ȡ����ı任��Ϣ����
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//����ǰ�ľ������ø��任��Ϣ����

	if(body->isActive())
	{
		cylinderSide->mFlagK = 0;
		bottomCircle->mFlagK = 0;
	}
	else
	{
		cylinderSide->mFlagK = 1;
		bottomCircle->mFlagK = 1;
	}

	//�����
	MatrixState::pushMatrix();//�����ֳ�
	cylinderSide->drawSelf(texId_cylinderSide);
	MatrixState::popMatrix();//�ָ��ֳ�

	//����Բ
	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, -offset_topCircle, 0);//ִ��ƽ��
	MatrixState::rotate(180,1,0,0);
	bottomCircle->drawSelf(texId_bottomCircle);
	MatrixState::popMatrix();//�ָ��ֳ�

	//����Բ
	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, offset_topCircle, 0);//ִ��ƽ��
	bottomCircle->drawSelf(texId_topCircle);
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}

void TexCylinderShape::drawSelfNoBody()
{
	MatrixState::pushMatrix();//�����ֳ�

	//�����
	MatrixState::pushMatrix();//�����ֳ�
	cylinderSide->drawSelf(texId_cylinderSide);
	MatrixState::popMatrix();//�ָ��ֳ�

	//����Բ
	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, -offset_topCircle, 0);//ִ��ƽ��
	MatrixState::rotate(180,1,0,0);
	bottomCircle->drawSelf(texId_bottomCircle);
	MatrixState::popMatrix();//�ָ��ֳ�

	//����Բ
	MatrixState::pushMatrix();//�����ֳ�
	MatrixState::translate(0, offset_topCircle, 0);//ִ��ƽ��
	bottomCircle->drawSelf(texId_topCircle);
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}

btRigidBody* TexCylinderShape::getBody()
{
	return body;
}
