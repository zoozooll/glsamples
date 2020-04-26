#include "TexCylinderShape.h"

TexCylinderShape::TexCylinderShape(
		int texId_bottomCircleIn,//底部圆形的纹理id
		int texId_topCircleIn,//顶部圆形的纹理id
		int texId_cylinderSideIn,//圆柱侧面的纹理id
		int n,//切分份数
		btVector3 halfExtents,//半区域
		btDynamicsWorld *dynamicsWorld,
		btScalar mass,
		btVector3 pos,
		btScalar restitutionIn,
		btScalar frictionIn)
{
	bool isDynamic = (mass != 0);//物体是否可以运动
	btVector3 localInertia = btVector3(0, 0, 0);//惯性向量
	btCollisionShape* colShape = new btCylinderShape(halfExtents);
	if(isDynamic) //如果物体可以运动
	{
		colShape->calculateLocalInertia(mass, localInertia);//计算惯性
	}
	btTransform startTransform = btTransform();//创建刚体的初始变换对象
	startTransform.setIdentity();//变换初始化
	startTransform.getOrigin().setValue(pos.x(), pos.y(), pos.z());//设置初始的位置
	//创建刚体的运动状态对象
	btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
	//创建刚体信息对象
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
	rbInfo.m_restitution = restitutionIn;//设置反弹系数
	rbInfo.m_friction = frictionIn;//设置摩擦系数
	body = new btRigidBody(rbInfo);//创建刚体

	dynamicsWorld->addRigidBody(body);//将刚体添加进物理世界

	this->m_dynamicsWorld = dynamicsWorld;

	bottomCircle = new Circle(
			texId_bottomCircleIn,
			halfExtents.x(),//半径
			n,//圆被切割的份数
			0//-halfExtents.y()//圆的高度
			);//底圆   圆是朝上的
	offset_topCircle = halfExtents.y();
	cylinderSide = new CylinderSide(
			texId_cylinderSideIn,
			halfExtents.x(),//半径
			n,//三角形总数
			halfExtents.y()*2//圆柱的高度
			);

	this->texId_bottomCircle = texId_bottomCircleIn;
	this->texId_topCircle = texId_topCircleIn;
	this->texId_cylinderSide = texId_cylinderSideIn;
}

TexCylinderShape::TexCylinderShape(
		int texId_bottomCircleIn,//底部圆形的纹理id
		int texId_topCircleIn,//顶部圆形的纹理id
		int texId_cylinderSideIn,//圆柱侧面的纹理id
		int n,//切分份数
		btVector3 halfExtents//半区域
		)
{

	bottomCircle = new Circle(
			texId_bottomCircleIn,
			halfExtents.x(),//半径
			n,//圆被切割的份数
			0//-halfExtents.y()//圆的高度
			);//底圆   圆是朝上的
	offset_topCircle = halfExtents.y();
	cylinderSide = new CylinderSide(
			texId_cylinderSideIn,
			halfExtents.x(),//半径
			n,//三角形总数
			halfExtents.y()*2//圆柱的高度
			);

	this->texId_bottomCircle = texId_bottomCircleIn;
	this->texId_topCircle = texId_topCircleIn;
	this->texId_cylinderSide = texId_cylinderSideIn;
}

void TexCylinderShape::drawSelf()
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;									//获取这个箱子的变换信息对象
	trans = body->getWorldTransform();				//获取刚体的变换信息对象
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//将当前的矩阵设置给变换信息对象

	if(body->isActive())
	{
		cylinderSide->mFlagK = 0;
		bottomCircle->mFlagK = 0;
	}
	else
	{
		cylinderSide->mFlagK = 1;
		bottomCircle->mFlagK = 1;
	}

	//画侧边
	MatrixState::pushMatrix();//保护现场
	cylinderSide->drawSelf(texId_cylinderSide);
	MatrixState::popMatrix();//恢复现场

	//画底圆
	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, -offset_topCircle, 0);//执行平移
	MatrixState::rotate(180,1,0,0);
	bottomCircle->drawSelf(texId_bottomCircle);
	MatrixState::popMatrix();//恢复现场

	//画顶圆
	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, offset_topCircle, 0);//执行平移
	bottomCircle->drawSelf(texId_topCircle);
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}

void TexCylinderShape::drawSelfNoBody()
{
	MatrixState::pushMatrix();//保护现场

	//画侧边
	MatrixState::pushMatrix();//保护现场
	cylinderSide->drawSelf(texId_cylinderSide);
	MatrixState::popMatrix();//恢复现场

	//画底圆
	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, -offset_topCircle, 0);//执行平移
	MatrixState::rotate(180,1,0,0);
	bottomCircle->drawSelf(texId_bottomCircle);
	MatrixState::popMatrix();//恢复现场

	//画顶圆
	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, offset_topCircle, 0);//执行平移
	bottomCircle->drawSelf(texId_topCircle);
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}

btRigidBody* TexCylinderShape::getBody()
{
	return body;
}
