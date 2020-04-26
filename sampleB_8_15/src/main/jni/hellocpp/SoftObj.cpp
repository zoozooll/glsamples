/*
 * SoftObj.cpp
 *
 *  Created on: 2015��1��31��
 *      Author: Nr
 */

#include "SoftObj.h"
#include "../myEncapsulation/MatrixState.h"

SoftObj::SoftObj(btSoftRigidDynamicsWorld* m_dynamicsWorld,
		btSoftBodyWorldInfo& m_softBodyWorldInfo,
		btVector3 pos,
		float* m_gVertices,
		int m_nums_ver,
		int* m_gIndices,
		int m_nums_ind,
		float scale
		)
{
	// TODO Auto-generated constructor stub
	this->m_dynamicsWorld = m_dynamicsWorld;//��¼��������ָ��
	this->m_softBodyWorldInfo = m_softBodyWorldInfo;//��¼����������Ϣ
	this->m_pos = pos;//��¼λ������
	this->m_gVertices = m_gVertices;//��¼��������
	for(int i = 0; i<m_nums_ver; i++)//������������
	{
		this->m_gVertices[i] *= scale;//���Ķ�������
	}
	this->m_gIndices = m_gIndices;//��¼��������
	this->m_numsTriangles = m_nums_ind / 3;//���������θ���

	initObj();//��ʼ�����巽��
	this->m_numT = psb->m_faces.size();//�����θ���

	this->m_vertices=new float[m_numT*3*3];//������������
	this->initVertex();//��ʼ����������
	this->asbv = new AnyShapeNoTexture(m_vertices,m_numT*3*3);//�������ƶ���

}

void SoftObj::initVertex()
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

void SoftObj::initObj()
{
	//ˮ��
	psb=createSoftBody(m_pos,btVector3(0,SIMD_PI/2,0));//���ô������巽��
	psb->generateClusters(8);
	psb->m_cfg.kDF=1;
}

btSoftBody*	SoftObj::createSoftBody(const btVector3& x,const btVector3& a)
{
	btSoftBody*	psb=btSoftBodyHelpers::CreateFromTriMesh(m_softBodyWorldInfo,m_gVertices,&m_gIndices[0],m_numsTriangles);//���������������巽��
	btSoftBody::Material*	pm=psb->appendMaterial();//��ȡ�������
	pm->m_kLST				=	1;//�������������Ϣ
	psb->generateBendingConstraints(2,pm);//��������ϵ��
	psb->m_cfg.piterations	=	2;
	psb->m_cfg.collisions	=	btSoftBody::fCollision::CL_SS+btSoftBody::fCollision::CL_RS;
	psb->randomizeConstraints();//�����������ϵ��
	psb->rotate(btQuaternion(a[0],a[1],a[2]));//�����������ת�任
	psb->translate(x);//���������ƽ�Ʊ任
	psb->setTotalMass(10,true);//��������������
	psb->generateClusters(64);//��������Ⱥ����
	m_dynamicsWorld->addSoftBody(psb);//��������ӵ�����������
	return(psb);//��������ָ��
}

//SoftObj::~SoftObj() {
//	// TODO Auto-generated destructor stub
//}

void SoftObj::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;
	trans = psb->getWorldTransform();//��ȡ����ı任��Ϣ����
	trans.getOpenGLMatrix(MatrixState::currMatrix);//����ǰ�ľ������ø��任��Ϣ����

//	�����
	MatrixState::pushMatrix();//�����ֳ�
	initVertex();//���¶�������
	this->asbv->drawSelf();//������
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}

btRigidBody* SoftObj::getBody()
{

}
