#include <math.h>
#include "TexPlane.h"
#include "MatrixState.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

TexPlane::TexPlane(float UNIT_SIZE,int texId,
		btCollisionShape *planeShape,
		btDynamicsWorld *dynamicsWorld,
		btScalar cx,btScalar cy,btScalar cz,
		btScalar restitutionIn,
		btScalar frictionIn)
{
	//��������ĳ�ʼ�任����
	btTransform groundTransform =  btTransform();
	groundTransform.setIdentity();//���ñ任�����һ����	//�任��ʼ��
	groundTransform.getOrigin().setValue(cx, cy, cz);//���ñ任�����ԭ��
	btVector3 localInertia = btVector3(0, 0, 0);//��������
	//����������˶�״̬����
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	//����������Ϣ����
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0,myMotionState,planeShape,localInertia);
	rbInfo.m_restitution = restitutionIn;//���÷���ϵ��
	rbInfo.m_friction = frictionIn;//����Ħ��ϵ��
	//��������
	body = new btRigidBody(rbInfo);
	//��������ӽ���������
	dynamicsWorld->addRigidBody(body);

	this->UNIT_SIZE = UNIT_SIZE;
	this->texId = texId;
	tp = new TexturePlane(
    		1, //���
    		1, //
    		0, //λ����
    		UNIT_SIZE//��λ����
			);
}



void TexPlane::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;
	trans = body->getWorldTransform();
	trans.getOpenGLMatrix(MatrixState::currMatrix);

	MatrixState::pushMatrix();//�����ֳ�
	tp->drawSelf(texId);
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�


}

btRigidBody* TexPlane::getBody()
{
	return body;
}
