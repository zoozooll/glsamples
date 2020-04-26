/*
 * SoftCloth.cpp
 *
 *  Created on: 2015��1��31��
 *      Author: Nr
 */

#include "SoftCloth.h"
#include "../myEncapsulation/MatrixState.h"

SoftCloth::SoftCloth(btSoftRigidDynamicsWorld* m_dynamicsWorld,
		btSoftBodyWorldInfo& m_softBodyWorldInfo,
		btVector3 pos
		)
{
	// TODO Auto-generated constructor stub
	this->m_dynamicsWorld = m_dynamicsWorld;//��¼��������ָ��
	this->m_softBodyWorldInfo = m_softBodyWorldInfo;//��¼����������Ϣ
	this->m_pos = pos;//��¼λ������
	initCloth();//����������
	this->m_numT = psb->m_faces.size();//��ȡ�����θ���
	this->m_vertices=new float[m_numT*3*3];//������������
	this->initVertex();//���ó�ʼ����������

	this->m_asbv = new AnyShapeNoTexture(m_vertices,m_numT*3*3);//�������ƶ���

}

void SoftCloth::initVertex()
{
	int count=0;//���������
	for(int i=0;i<m_numT;++i)//��������������
	{
		const btSoftBody::Face&	f=psb->m_faces[i];//��ȡ�����ϵ�һ����
		const btVector3	x[]={f.m_n[0]->m_x,f.m_n[1]->m_x,f.m_n[2]->m_x};//��ȡ����������
		m_vertices[count++]=x[0].x();//��ȡ��һ�����x����
		m_vertices[count++]=x[0].y();//��ȡ��һ�����y����
		m_vertices[count++]=x[0].z();//��ȡ��һ�����z����
		m_vertices[count++]=x[1].x();//��ȡ�ڶ������x����
		m_vertices[count++]=x[1].y();//��ȡ�ڶ������y����
		m_vertices[count++]=x[1].z();//��ȡ�ڶ������z����
		m_vertices[count++]=x[2].x();//��ȡ���������x����
		m_vertices[count++]=x[2].y();//��ȡ���������y����
		m_vertices[count++]=x[2].z();//��ȡ���������z����
	}
}

void SoftCloth::initCloth()
{
	//��
	const btScalar s=8;//���߶�
	psb=btSoftBodyHelpers::CreatePatch(	m_softBodyWorldInfo,btVector3(-s,8,+s),
		btVector3(+s,8,+s),
		btVector3(-s,8,-s),
		btVector3(+s,8,-s),
		8,8,
		1+2+4+8,true);
	psb->getCollisionShape()->setMargin(0.5);//��������ײ��״��Եֵ
	btSoftBody::Material* pm=psb->appendMaterial();//��ȡ����Ĳ���
	pm->m_kLST=0.4;//
	psb->generateBendingConstraints(2,pm);//
	psb->setTotalMass(150);//��������������
	psb->translate(m_pos);//��������λ��
	m_dynamicsWorld->addSoftBody(psb);//������ӵ�����������
}


//SoftCloth::~SoftCloth() {
//	// TODO Auto-generated destructor stub
//}

void SoftCloth::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;
	trans = psb->getWorldTransform();//��ȡ����ı任��Ϣ����
	trans.getOpenGLMatrix(MatrixState::currMatrix);//����ǰ�ľ������ø��任��Ϣ����

//	LOGI("weizhi=%f,%f,%f",trans.getOrigin().x(),trans.getOrigin().y(),trans.getOrigin().z());

//
	MatrixState::pushMatrix();//�����ֳ�
	initVertex();//���¶�������
	this->m_asbv->drawSelf();//������
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}

btRigidBody* SoftCloth::getBody()
{

}
