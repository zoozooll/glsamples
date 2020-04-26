/*
 * SoftRope.cpp
 *
 *  Created on: 2015年1月31日
 *      Author: Nr
 */

#include "SoftRope.h"
#include "../myEncapsulation/MatrixState.h"

SoftRope::SoftRope(btSoftRigidDynamicsWorld* m_dynamicsWorld,
		btSoftBodyWorldInfo& m_softBodyWorldInfo,
		btVector3 m_pos,
		int ropeId,//纹理id
		int ballId,
		int boxId
		)
{
	// TODO Auto-generated constructor stub
	this->m_dynamicsWorld = m_dynamicsWorld;//记录物理世界指针
	this->m_softBodyWorldInfo = m_softBodyWorldInfo;//记录物理世界信息
	this->m_ropeId = ropeId;//记录纹理id
	this->m_ballId = ballId;//记录纹理id
	this->m_boxId = boxId;//记录纹理id
	this->m_pos = m_pos;//记录小球位置
	init();//调用创建绳索、小球和木块的方法
}

void SoftRope::initVertex()
{
	for(int i = 0; i < psb->m_nodes.size(); i++)//遍历绳索上所有的点
	{
		btSoftBody::Node tempNode = psb->m_nodes.at(i);//获取绳索上的节点
		m_vec_rope[i] = tempNode.m_x;//设置绳索顶点数据
	}
}

void SoftRope::init()
{
	float lengthRope = 10.0f;//绳索长度
	tbBall = new TexBall(0.2f,m_ballId,m_dynamicsWorld,0.0f,m_pos,0,0);//创建小球类对象
	tbCuboid = new 	TexCuboid(
			m_dynamicsWorld,//物理世界对象
			btVector3(2,2,2),//长方体的半区域
			50,//长方体的质量
			m_pos + btVector3(2+lengthRope, 0, 0),
			0.5,//恢复系数
			0.5,//摩擦系数
			1.0f,//单位长度
			m_boxId,//前侧纹理图id
			m_boxId,//后侧纹理图id
			m_boxId,//上侧纹理图id
			m_boxId,//下侧纹理图id
			m_boxId,//左侧纹理图id
			m_boxId//右侧纹理图id
	);//创建木块对象
	psb=btSoftBodyHelpers::CreateRope(this->m_softBodyWorldInfo,m_pos,m_pos+btVector3(lengthRope,0,0),8,1);//创建绳索软体
	psb->setTotalMass(50);//设置绳索质量
	m_dynamicsWorld->addSoftBody(psb);//将绳索软体添加到物理世界中
	psb->appendAnchor(psb->m_nodes.size()-1,tbCuboid->getBody());//设置绳索软体和木块之间的锚点

	m_vec_rope = new btVector3[psb->m_nodes.size()];//创建绳索顶点数组
	initVertex();//初始化顶点数组
	dr = new DrawRope(m_vec_rope, psb->m_nodes.size(), m_ropeId);//创建绘制绳索类对象
}

//SoftRope::~SoftRope() {
//	// TODO Auto-generated destructor stub
//}

void SoftRope::drawSelf()
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;
	trans = psb->getWorldTransform();				//获取刚体的变换信息对象
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//将当前的矩阵设置给变换信息对象

//	LOGI("weizhi=%f,%f,%f",trans.getOrigin().x(),trans.getOrigin().y(),trans.getOrigin().z());


	MatrixState::pushMatrix();//保护现场
	initVertex();//更新顶点数据
	dr->drawSelf();//绘制绳索
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	tbCuboid->drawSelf();//绘制木块
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	tbBall->drawSelf();//绘制小球
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}

btRigidBody* SoftRope::getBody()
{

}
