/*
 * SoftCloth.cpp
 *
 *  Created on: 2015年1月31日
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
	this->m_dynamicsWorld = m_dynamicsWorld;//记录物理世界指针
	this->m_softBodyWorldInfo = m_softBodyWorldInfo;//记录物理世界信息
	this->m_pos = pos;//记录位置坐标
	initCloth();//调用软布方法
	this->m_numT = psb->m_faces.size();//获取三角形个数
	this->m_vertices=new float[m_numT*3*3];//创建顶点数组
	this->initVertex();//调用初始化顶点数组

	this->m_asbv = new AnyShapeNoTexture(m_vertices,m_numT*3*3);//创建绘制对象

}

void SoftCloth::initVertex()
{
	int count=0;//顶点计数器
	for(int i=0;i<m_numT;++i)//遍历所有三角形
	{
		const btSoftBody::Face&	f=psb->m_faces[i];//获取软体上的一个面
		const btVector3	x[]={f.m_n[0]->m_x,f.m_n[1]->m_x,f.m_n[2]->m_x};//获取组成面的数组
		m_vertices[count++]=x[0].x();//获取第一个点的x坐标
		m_vertices[count++]=x[0].y();//获取第一个点的y坐标
		m_vertices[count++]=x[0].z();//获取第一个点的z坐标
		m_vertices[count++]=x[1].x();//获取第二个点的x坐标
		m_vertices[count++]=x[1].y();//获取第二个点的y坐标
		m_vertices[count++]=x[1].z();//获取第二个点的z坐标
		m_vertices[count++]=x[2].x();//获取第三个点的x坐标
		m_vertices[count++]=x[2].y();//获取第三个点的y坐标
		m_vertices[count++]=x[2].z();//获取第三个点的z坐标
	}
}

void SoftCloth::initCloth()
{
	//软布
	const btScalar s=8;//软布高度
	psb=btSoftBodyHelpers::CreatePatch(	m_softBodyWorldInfo,btVector3(-s,8,+s),
		btVector3(+s,8,+s),
		btVector3(-s,8,-s),
		btVector3(+s,8,-s),
		8,8,
		1+2+4+8,true);
	psb->getCollisionShape()->setMargin(0.5);//设置软布碰撞形状边缘值
	btSoftBody::Material* pm=psb->appendMaterial();//获取软体的材质
	pm->m_kLST=0.4;//
	psb->generateBendingConstraints(2,pm);//
	psb->setTotalMass(150);//设置软布的总质量
	psb->translate(m_pos);//设置软布的位置
	m_dynamicsWorld->addSoftBody(psb);//将软布添加到物理世界中
}


//SoftCloth::~SoftCloth() {
//	// TODO Auto-generated destructor stub
//}

void SoftCloth::drawSelf()
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;
	trans = psb->getWorldTransform();//获取刚体的变换信息对象
	trans.getOpenGLMatrix(MatrixState::currMatrix);//将当前的矩阵设置给变换信息对象

//	LOGI("weizhi=%f,%f,%f",trans.getOrigin().x(),trans.getOrigin().y(),trans.getOrigin().z());

//
	MatrixState::pushMatrix();//保护现场
	initVertex();//更新顶点数据
	this->m_asbv->drawSelf();//绘制软布
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}

btRigidBody* SoftCloth::getBody()
{

}
