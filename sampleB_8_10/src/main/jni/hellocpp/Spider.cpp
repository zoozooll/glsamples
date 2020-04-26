#include "Spider.h"
#include "../myEncapsulation/TexCapsule.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923
#endif

Spider::Spider(btDynamicsWorld* ownerWorld,
			btVector3 positionOffset,//const
			btScalar scale,
			int boxTexId,
			int ballTexId
			)
{
	this->m_ownerWorld = ownerWorld;//��ȫ�ֵ���������ָ�븳ֵ

	float fBodySize  = 0.25f * scale;//���
	float fLegLength = 0.45f * scale;//�ȵĳ���
	float fForeLegLength = 0.75f * scale;//ǰ�ȵĳ���
	int i;
	float fHeight = 0.5 * scale;//�߶�
	btTransform offset;//�任����
	offset.setIdentity();//�任�����ʼ��
	offset.setOrigin(positionOffset);//���任�����������

	// root
	btVector3 vRoot = btVector3(btScalar(0.), btScalar(fHeight), btScalar(0.));//�м�����λ��
	btTransform transform;//�м����ı任����
	transform.setIdentity();//�任�����ʼ��
	transform.setOrigin(vRoot);//���任��������λ��
	tbTemp = new TexCapsule(
		1.0f,//����
		fBodySize,//�뾶
		0.10 * scale,//�߶�
		offset*transform,//λ��
		0.0f,//�ָ�ϵ��
		0.0f,//Ħ��ϵ��
		ownerWorld,//��������ָ��
		boxTexId,//���Ҳ������id
		ballTexId,//�ײ�Բ����id
		ballTexId,//����Բ����id
		30//���зֵķ���  n
		);//�����м�������
	tca.push_back(tbTemp);//���м������뵽�б���
	m_bodies[0] = tbTemp->getBody();//��¼����ָ��
	// legs
	for ( i=0; i<NUM_LEGS; i++)
	{
		float fAngle = 2 * M_PI * i / NUM_LEGS;
		float fSin = sin(fAngle);
		float fCos = cos(fAngle);

		transform.setIdentity();
		btVector3 vBoneOrigin = btVector3(btScalar(fCos*(fBodySize+0.5*fLegLength)), btScalar(fHeight), btScalar(fSin*(fBodySize+0.5*fLegLength)));
		transform.setOrigin(vBoneOrigin);

		// thigh
		btVector3 vToBone = (vBoneOrigin - vRoot).normalize();
		btVector3 vAxis = vToBone.cross(btVector3(0, 1, 0));
		transform.setRotation(btQuaternion(vAxis, M_PI_2));
//		m_bodies[1+2*i] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[1+2*i]);
		tbTemp = new TexCapsule(
			1.0f,//����
			0.10f * scale,//�뾶
			fLegLength,//�߶�
			offset*transform,//λ��
			0.5f,//�ָ�ϵ��
			0.5f,//Ħ��ϵ��
			ownerWorld,//��������ָ��
			boxTexId,//���Ҳ������id
			ballTexId,//�ײ�Բ����id
			ballTexId,//����Բ����id
			30//���зֵķ���  n
			);
		tca.push_back(tbTemp);//������������뵽�б���

		m_bodies[1+2*i] = tbTemp->getBody();

		// shin
		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(fCos*(fBodySize+fLegLength)), btScalar(fHeight-0.5*fForeLegLength), btScalar(fSin*(fBodySize+fLegLength))));
//		m_bodies[2+2*i] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[2+2*i]);
		tbTemp = new TexCapsule(
			1.0f,//����
			0.08f * scale,//�뾶
			fForeLegLength,//�߶�
			offset*transform,//λ��
			0.5f,//�ָ�ϵ��
			0.5f,//Ħ��ϵ��
			ownerWorld,//��������ָ��
			boxTexId,//���Ҳ������id
			ballTexId,//�ײ�Բ����id
			ballTexId,//����Բ����id
			30//���зֵķ���  n
			);
		tca.push_back(tbTemp);//������������뵽�б���

		m_bodies[2+2*i] = tbTemp->getBody();
	}

	// Setup some damping on the m_bodies
	for (i = 0; i < BODYPART_COUNT; ++i)//�������еĸ���
	{
		m_bodies[i]->setDamping(0.05, 0.85);//���ø��������
		m_bodies[i]->setDeactivationTime(0.8);//���ø���ı���ʱ��
		//m_bodies[i]->setSleepingThresholds(1.6, 2.5);
		m_bodies[i]->setSleepingThresholds(0.5f, 0.5f);//����˯����ֵ
	}


	//
	// Setup the constraints
	//
	btGeneric6DofConstraint * joint6DOF;//�����ɶȹؽ�ָ��

	btTransform localA, localB, localC;//�仯����

	bool useLinearReferenceFrameA = true;//��Ӧ��ϵ��־

	for ( i=0; i<NUM_LEGS; i++)//�������е��ȸ���
	{
		float fAngle = 2 * M_PI * i / NUM_LEGS;//����Ƕ�
		float fSin = sin(fAngle);//�Ƕȵ�����ֵ
		float fCos = cos(fAngle);//�Ƕȵ�����ֵ

		// hip joints
		localA.setIdentity(); //�任�����ʼ��
		localB.setIdentity();//�任�����ʼ��
		localA.getBasis().setEulerZYX(0,-fAngle,0);	//����ŷ����
		localA.setOrigin(btVector3(btScalar(fCos*fBodySize), btScalar(0.), btScalar(fSin*fBodySize)));//����ԭ��
		localB = m_bodies[1+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;//����任����
		joint6DOF = new btGeneric6DofConstraint(*m_bodies[0], *m_bodies[1+2*i], localA, localB,useLinearReferenceFrameA);//���������ɶȹؽڶ���
		joint6DOF->setAngularLowerLimit(btVector3(-SIMD_PI*0.05f,-SIMD_EPSILON,-SIMD_PI*0.05f));//���������ɶȹؽڵ�ת��������
		joint6DOF->setAngularUpperLimit(btVector3(SIMD_PI*0.05f,SIMD_EPSILON,SIMD_PI*0.051));//���������ɶȹؽڵ�ת��������
		m_joints[2*i] = joint6DOF;//��¼�ؽڶ���
		m_ownerWorld->addConstraint(m_joints[2*i], true);//���ؽڶ�����ӵ�����������

		// knee joints
		localA.setIdentity(); //�任�����ʼ��
		localB.setIdentity(); //�任�����ʼ��
		localC.setIdentity();//�任�����ʼ��
		localA.getBasis().setEulerZYX(0,-fAngle,0);	localA.setOrigin(btVector3(btScalar(fCos*(fBodySize+fLegLength)), btScalar(0.), btScalar(fSin*(fBodySize+fLegLength))));//����ŷ����
		localB = m_bodies[1+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;//����任����
		localC = m_bodies[2+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;//����任����
		joint6DOF = new btGeneric6DofConstraint(*m_bodies[1+2*i], *m_bodies[2+2*i], localB, localC,useLinearReferenceFrameA);//���������ɶȹؽڶ���
		joint6DOF->setAngularLowerLimit(btVector3(-SIMD_PI*0.05f,-SIMD_EPSILON,-SIMD_PI*0.05f));//���������ɶȹؽڵ�ת��������
		joint6DOF->setAngularUpperLimit(btVector3(SIMD_PI*0.05f,SIMD_EPSILON,SIMD_PI*0.05f));//���������ɶȹؽڵ�ת��������
		m_joints[1+2*i] = joint6DOF;//��¼�ؽڶ���
		m_ownerWorld->addConstraint(m_joints[1+2*i], true);//���ؽڶ�����ӵ�����������
	}

}


btRigidBody* Spider::getBody(){}

void Spider::drawSelf()
{
    for ( int i = 0; i < tca.size(); ++i )
    {
    	tca[i]->drawSelf();//�ص�����Ļ��Ʒ���
    }
}

