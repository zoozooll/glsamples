#include "TexCapsule.h"
#include "ShaderUtil.h"
#include "Bullet/BulletCollision/CollisionShapes/btCapsuleShape.h"

TexCapsule::TexCapsule(
		btScalar mass,//质量
		float radiusIn,//半径
		float heightIn,//高度
		btVector3 pos,//位置
		btScalar restitutionIn,//恢复系数
		btScalar frictionIn,//摩擦系数
		btDynamicsWorld *dynamicsWorld,//物理世界指针
		int capuleSideTexIdIn,//胶囊侧边纹理id
		int buttomBallTexIdIn,//底部圆纹理id
		int topBallTexIdIn,//顶部圆纹理id
		int n//被切分的份数  n
		)
{
	bool isDynamic = (mass != 0);//物体是否可以运动

	btCapsuleShape* coneShape = new btCapsuleShape(radiusIn,heightIn);

	btVector3 localInertia = btVector3(0, 0, 0);//惯性向量
	if(isDynamic)
	{
		coneShape->calculateLocalInertia(mass, localInertia);
	}
	btTransform transform = btTransform();//创建刚体的初始变换对象
	transform.setIdentity();//变换对象初始化
	transform.getOrigin().setValue(pos.x(),pos.y(),pos.z());//设置变换对象的位置
	//创建刚体运动状态对象
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	//创建刚体信息对象
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, coneShape, localInertia);
	rbInfo.m_restitution = restitutionIn;//设置反弹系数
	rbInfo.m_friction = frictionIn;//设置摩擦系数
	body = new btRigidBody(rbInfo);//创建刚体
	dynamicsWorld->addRigidBody(body);//将刚体添加进物理世界

	this->radius = radiusIn;//半径
	this->height = heightIn;//高度
	this->capuleSideTexId = capuleSideTexIdIn;//胶囊侧边纹理id
	this->topBallTexId = topBallTexIdIn;//底部圆纹理id
	this->buttomBallTexId = buttomBallTexIdIn;//顶部圆纹理id

	capuleSide = new CylinderSide(
			capuleSideTexId,
			radius,//半径
			n,//切分的份数
			height//圆柱的高度
			);//轴为y轴的圆柱侧边
	bottomBall = new Ball(
			topBallTexId,
			radius//半径
			);
}

TexCapsule::TexCapsule(
		float radiusIn,//半径
		float heightIn,//高度
		int capuleSideTexIdIn,//胶囊侧边纹理id
		int buttomBallTexIdIn,//底部圆纹理id
		int topBallTexIdIn,//顶部圆纹理id
		int n//被切分的份数  n
		)
{
	this->radius = radiusIn;//半径
	this->height = heightIn;//高度
	this->capuleSideTexId = capuleSideTexIdIn;//胶囊侧边纹理id
	this->topBallTexId = topBallTexIdIn;//底部圆纹理id
	this->buttomBallTexId = buttomBallTexIdIn;//顶部圆纹理id

	capuleSide = new CylinderSide(
			capuleSideTexId,
			radius,//半径
			n,//切分的份数
			height//圆柱的高度
			);//轴为y轴的圆柱侧边
	bottomBall = new Ball(
			topBallTexId,
			radius//半径
			);
}

void TexCapsule::drawSelf()
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;									//获取这个箱子的变换信息对象
	trans = body->getWorldTransform();				//获取刚体的变换信息对象
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//将当前的矩阵设置给变换信息对象

	if(body->isActive())
	{
		capuleSide->mFlagK = 0;
		bottomBall->mFlagK = 0;
	}
	else
	{
		capuleSide->mFlagK = 1;
		bottomBall->mFlagK = 1;
	}

	//画侧边
	MatrixState::pushMatrix();//保护现场
	capuleSide->drawSelf(capuleSideTexId);
	MatrixState::popMatrix();//恢复现场

	//
	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, -height/2, 0);//执行平移
	bottomBall->drawSelf(topBallTexId);
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, height/2, 0);//执行平移
	bottomBall->drawSelf(topBallTexId);
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}

void TexCapsule::drawSelfNoBody()
{
	MatrixState::pushMatrix();//保护现场

	//画侧边
	MatrixState::pushMatrix();//保护现场
	capuleSide->drawSelf(capuleSideTexId);
	MatrixState::popMatrix();//恢复现场

	//
	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, -height/2, 0);//执行平移
	bottomBall->drawSelf(topBallTexId);
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, height/2, 0);//执行平移
	bottomBall->drawSelf(topBallTexId);
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}

btRigidBody* TexCapsule::getBody()
{
	return body;
}
