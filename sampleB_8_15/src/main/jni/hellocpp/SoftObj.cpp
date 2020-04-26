/*
 * SoftObj.cpp
 *
 *  Created on: 2015年1月31日
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
	this->m_dynamicsWorld = m_dynamicsWorld;//记录物理世界指针
	this->m_softBodyWorldInfo = m_softBodyWorldInfo;//记录物理世界信息
	this->m_pos = pos;//记录位置坐标
	this->m_gVertices = m_gVertices;//记录顶点数组
	for(int i = 0; i<m_nums_ver; i++)//遍历顶点数组
	{
		this->m_gVertices[i] *= scale;//更改顶点数据
	}
	this->m_gIndices = m_gIndices;//记录索引数组
	this->m_numsTriangles = m_nums_ind / 3;//计算三角形个数

	initObj();//初始化软体方法
	this->m_numT = psb->m_faces.size();//三角形个数

	this->m_vertices=new float[m_numT*3*3];//创建顶点数组
	this->initVertex();//初始化顶点数据
	this->asbv = new AnyShapeNoTexture(m_vertices,m_numT*3*3);//创建绘制对象

}

void SoftObj::initVertex()
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

void SoftObj::initObj()
{
	//水袋
	psb=createSoftBody(m_pos,btVector3(0,SIMD_PI/2,0));//调用创建软体方法
	psb->generateClusters(8);
	psb->m_cfg.kDF=1;
}

btSoftBody*	SoftObj::createSoftBody(const btVector3& x,const btVector3& a)
{
	btSoftBody*	psb=btSoftBodyHelpers::CreateFromTriMesh(m_softBodyWorldInfo,m_gVertices,&m_gIndices[0],m_numsTriangles);//创建三角形组软体方法
	btSoftBody::Material*	pm=psb->appendMaterial();//获取软体材质
	pm->m_kLST				=	1;//设置软体材质信息
	psb->generateBendingConstraints(2,pm);//设置弯曲系数
	psb->m_cfg.piterations	=	2;
	psb->m_cfg.collisions	=	btSoftBody::fCollision::CL_SS+btSoftBody::fCollision::CL_RS;
	psb->randomizeConstraints();//设置软体随机系数
	psb->rotate(btQuaternion(a[0],a[1],a[2]));//对软体进行旋转变换
	psb->translate(x);//对软体进行平移变换
	psb->setTotalMass(10,true);//设置软体总质量
	psb->generateClusters(64);//设置软体群集数
	m_dynamicsWorld->addSoftBody(psb);//将软体添加到物理世界中
	return(psb);//返回软体指针
}

//SoftObj::~SoftObj() {
//	// TODO Auto-generated destructor stub
//}

void SoftObj::drawSelf()
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;
	trans = psb->getWorldTransform();//获取刚体的变换信息对象
	trans.getOpenGLMatrix(MatrixState::currMatrix);//将当前的矩阵设置给变换信息对象

//	画侧边
	MatrixState::pushMatrix();//保护现场
	initVertex();//更新顶点数据
	this->asbv->drawSelf();//绘制软布
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}

btRigidBody* SoftObj::getBody()
{

}
