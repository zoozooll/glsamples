/*
 * SoftRope.cpp
 *
 *  Created on: 2015��1��31��
 *      Author: Nr
 */

#include "SoftRope.h"
#include "../myEncapsulation/MatrixState.h"

SoftRope::SoftRope(btSoftRigidDynamicsWorld* m_dynamicsWorld,
		btSoftBodyWorldInfo& m_softBodyWorldInfo,
		btVector3 m_pos,
		int ropeId,//����id
		int ballId,
		int boxId
		)
{
	// TODO Auto-generated constructor stub
	this->m_dynamicsWorld = m_dynamicsWorld;//��¼��������ָ��
	this->m_softBodyWorldInfo = m_softBodyWorldInfo;//��¼����������Ϣ
	this->m_ropeId = ropeId;//��¼����id
	this->m_ballId = ballId;//��¼����id
	this->m_boxId = boxId;//��¼����id
	this->m_pos = m_pos;//��¼С��λ��
	init();//���ô���������С���ľ��ķ���
}

void SoftRope::initVertex()
{
	for(int i = 0; i < psb->m_nodes.size(); i++)//�������������еĵ�
	{
		btSoftBody::Node tempNode = psb->m_nodes.at(i);//��ȡ�����ϵĽڵ�
		m_vec_rope[i] = tempNode.m_x;//����������������
	}
}

void SoftRope::init()
{
	float lengthRope = 10.0f;//��������
	tbBall = new TexBall(0.2f,m_ballId,m_dynamicsWorld,0.0f,m_pos,0,0);//����С�������
	tbCuboid = new 	TexCuboid(
			m_dynamicsWorld,//�����������
			btVector3(2,2,2),//������İ�����
			50,//�����������
			m_pos + btVector3(2+lengthRope, 0, 0),
			0.5,//�ָ�ϵ��
			0.5,//Ħ��ϵ��
			1.0f,//��λ����
			m_boxId,//ǰ������ͼid
			m_boxId,//�������ͼid
			m_boxId,//�ϲ�����ͼid
			m_boxId,//�²�����ͼid
			m_boxId,//�������ͼid
			m_boxId//�Ҳ�����ͼid
	);//����ľ�����
	psb=btSoftBodyHelpers::CreateRope(this->m_softBodyWorldInfo,m_pos,m_pos+btVector3(lengthRope,0,0),8,1);//������������
	psb->setTotalMass(50);//������������
	m_dynamicsWorld->addSoftBody(psb);//������������ӵ�����������
	psb->appendAnchor(psb->m_nodes.size()-1,tbCuboid->getBody());//�������������ľ��֮���ê��

	m_vec_rope = new btVector3[psb->m_nodes.size()];//����������������
	initVertex();//��ʼ����������
	dr = new DrawRope(m_vec_rope, psb->m_nodes.size(), m_ropeId);//�����������������
}

//SoftRope::~SoftRope() {
//	// TODO Auto-generated destructor stub
//}

void SoftRope::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;
	trans = psb->getWorldTransform();				//��ȡ����ı任��Ϣ����
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//����ǰ�ľ������ø��任��Ϣ����

//	LOGI("weizhi=%f,%f,%f",trans.getOrigin().x(),trans.getOrigin().y(),trans.getOrigin().z());


	MatrixState::pushMatrix();//�����ֳ�
	initVertex();//���¶�������
	dr->drawSelf();//��������
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::pushMatrix();//�����ֳ�
	tbCuboid->drawSelf();//����ľ��
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::pushMatrix();//�����ֳ�
	tbBall->drawSelf();//����С��
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}

btRigidBody* SoftRope::getBody()
{

}
