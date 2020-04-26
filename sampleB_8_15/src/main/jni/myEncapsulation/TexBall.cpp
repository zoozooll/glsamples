#include <math.h>
#include "TexBall.h"
#include "ShaderUtil.h"
#include "MatrixState.h"

TexBall::TexBall(float r,int texId,
		btDynamicsWorld *dynamicsWorld,
		btScalar mass,
		btVector3 pos,
		btScalar restitutionIn,
		btScalar frictionIn)
{
	bool isDynamic = (mass != 0);//�����Ƿ�����˶�
	btVector3 localInertia = btVector3(0, 0, 0);//��ʾ��������
	//����������״
	btCollisionShape* colShape = new btSphereShape(r);
	if(isDynamic)
	{
		colShape->calculateLocalInertia(mass, localInertia);//�������
	}
	btTransform startTransform = btTransform();//��������ĳ�ʼ�任����
	startTransform.setIdentity();//�任�����ʼ��
	startTransform.getOrigin().setValue(pos.x(), pos.y(), pos.z());//���ñ任�����λ��
	btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);//���������˶�״̬����

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);//����������Ϣ����
	rbInfo.m_restitution = restitutionIn;//���÷���ϵ��
	rbInfo.m_friction = frictionIn;//����Ħ��ϵ��
	body = new btRigidBody(rbInfo);//��������

	dynamicsWorld->addRigidBody(body);//��������ӽ���������

	this->r = r;//��¼��뾶
	this->texId = texId;//��¼����id

	ball = new Ball(
			texId,
			r
			);//�������������
}


void TexBall::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;									//��ȡ������ӵı任��Ϣ����
	trans = body->getWorldTransform();				//��ȡ����ı任��Ϣ����
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//����ǰ�ľ������ø��任��Ϣ����

	if(body->isActive())//�жϸ����Ƿ�Ϊ��̬
	{
		ball->mFlagK = 0;//��������������
	}
	else
	{
		ball->mFlagK = 1;//��������������
	}

	MatrixState::pushMatrix();//�����ֳ�
	ball->drawSelf(texId);//������
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}

btRigidBody* TexBall::getBody()
{
	return body;//���ظ���ָ��
}
