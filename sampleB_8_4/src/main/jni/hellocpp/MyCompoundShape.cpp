#include "MyCompoundShape.h"

MyCompoundShape::MyCompoundShape(
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
		)
{

	int scale = 4;//�������ű�
	btVector3 halfExtents(1,0.125,1);//Բ���İ�����
	halfExtents = halfExtents * scale;
	float radio = 0.05f * scale;//��ʾ����������İ뾶
	float height = 1.5f * scale;//��ʾ�����м�Բ���ĸ߶�

	btCollisionShape* top  = new btCylinderShape(halfExtents);//����Բ��
	btCollisionShape* pin  = new btCapsuleShape(radio,height);//��������
	top->setMargin(0.01);//����Բ���ı�Եֵ
	pin->setMargin(0.01);//���ý��ҵı�Եֵ
	btCompoundShape* compound = new btCompoundShape();//����������״����
	compound->addChildShape(btTransform::getIdentity(),top);//��Բ����״��ӵ�������״��
	compound->addChildShape(btTransform::getIdentity(),pin);//��������״��ӵ�������״��
	btVector3 localInertia;//��������
	top->calculateLocalInertia(mass,localInertia);//�������
	body = new btRigidBody(mass,0,compound,localInertia);//��������
	btTransform tr;
	tr.setIdentity();//�任�����ʼ��
	tr.setOrigin(pos);//����λ��
	body->setCenterOfMassTransform(tr);//�������ĵ�
	body->setAngularVelocity(btVector3(0,10,0));//���ý��ٶ�
	body->setLinearVelocity(btVector3(0,0,0.2));//�������ٶ�
	body->setFriction(btSqrt(1));//����Ħ��ϵ��
	dynamicsWorld->addRigidBody(body);//��������ӵ�����������
	body->setDamping(0.00001f,0.0001f);//��������

	tc = new TexCapsule(
			radio,//�뾶
			height,//�߶�
			texId_capusleSide,//���Ҳ������id
			texId_buttomBall,//�ײ�Բ����id
			texId_topBall,//����Բ����id
			30//���зֵķ���  n
			);//�������ƽ��Ҷ���
	tcs = new TexCylinderShape(
			texId_bottomCircle,//�ײ�Բ�ε�����id
			texId_topCircle,//����Բ�ε�����id
			texId_cylinderSide,//Բ�����������id
			30,//�зַ���
			halfExtents//������
			);//��ʼԲ������ƽ����y�ᣬԲ����ƽ��xoz�����ϵ�//��������Բ������

}

void MyCompoundShape::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;
	trans = body->getWorldTransform();				//��ȡ����ı任��Ϣ����
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//����ǰ�ľ������ø��任��Ϣ����

	if(body->isActive())//�жϸ����Ƿ�ɶ�
	{
		tc->capuleSide->mFlagK = 0;//���ý�����������
		tc->bottomBall->mFlagK = 0;//���ý�����������

		tcs->bottomCircle->mFlagK = 0;//����Բ����������
		tcs->cylinderSide->mFlagK = 0;//����Բ����������
	}
	else
	{
		tc->capuleSide->mFlagK = 1;//���ý�����������
		tc->bottomBall->mFlagK = 1;//���ý�����������

		tcs->bottomCircle->mFlagK = 1;//����Բ����������
		tcs->cylinderSide->mFlagK = 1;//����Բ����������
	}

	tc->drawSelfNoBody();//���ƽ���
	tcs->drawSelfNoBody();//����Բ��

	MatrixState::popMatrix();//�ָ��ֳ�
}

btRigidBody* MyCompoundShape::getBody()
{
	return body;//�����������ָ��
}
