#include "TexCone.h"
#include "ShaderUtil.h"
#include "Bullet/BulletCollision/CollisionShapes/btConeShape.h"

TexCone::TexCone(
	btScalar mass,//质量
	float radiusIn,//半径
	float heightIn,//高度
	btVector3 pos,//位置
	btScalar restitutionIn,//恢复系数
	btScalar frictionIn,//摩擦系数
	btDynamicsWorld *dynamicsWorld,//物理世界指针
	int sliderTexIdIn,//圆锥侧边纹理id
	int circleTexIdIn,//圆锥底圆纹理id
	int n//被切分的份数  n
	)
{
	bool isDynamic = (mass != 0);//物体是否可以运动

	btConeShape* coneShape = new btConeShape(radiusIn,heightIn);//创建圆锥形状

	btVector3 localInertia = btVector3(0, 0, 0);//惯性向量
	if(isDynamic)
	{
		coneShape->calculateLocalInertia(mass, localInertia);//计算惯性
	}
	btTransform transform = btTransform();//创建刚体的初始变换对象
	transform.setIdentity();//变换对象初始化
	transform.getOrigin().setValue(pos.x(),pos.y(),pos.z());//设置变换对象的位置
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);//创建刚体运动状态对象
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, coneShape, localInertia);//创建刚体信息对象
	rbInfo.m_restitution = restitutionIn;//设置反弹系数
	rbInfo.m_friction = frictionIn;//设置摩擦系数
	body = new btRigidBody(rbInfo);//创建刚体
	dynamicsWorld->addRigidBody(body);//将刚体添加进物理世界
	this->radius = radiusIn;//记录底圆半径
	this->height = heightIn;//记录圆锥高度
	this->sideTexId = sliderTexIdIn;//记录圆锥侧边纹理id
	this->circleTexId = circleTexIdIn;//记录圆锥底圆纹理id
	bottomCircle = new Circle(
			circleTexId,
			radiusIn,//半径
			n,//圆被切割的份数   30
			0//圆的高度  0
			);//xoz面上的圆   是朝上的圆
	coneSide = new ConeSide(
			sideTexId,
			radiusIn,//半径
			n,//切分的份数
			heightIn//圆柱的高度
			);//轴为y轴的圆柱侧边
}

void TexCone::drawSelf()
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;									//获取这个箱子的变换信息对象
	trans = body->getWorldTransform();				//获取刚体的变换信息对象
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//将当前的矩阵设置给变换信息对象

	if(body->isActive())//判断刚体当前是否可动
	{
		coneSide->mFlagK = 0;//设置圆锥侧面纹理属性
		bottomCircle->mFlagK = 0;//设置底圆纹理属性
	}
	else
	{
		coneSide->mFlagK = 1;//设置圆锥侧面纹理属性
		bottomCircle->mFlagK = 1;//设置底圆纹理属性
	}

	//画侧边
	MatrixState::pushMatrix();//保护现场
	coneSide->drawSelf(sideTexId);//绘制圆锥侧面
	MatrixState::popMatrix();//恢复现场

	//画底圆
	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, -height/2, 0);//执行平移
	MatrixState::rotate(180,1,0,0);//执行旋转
	bottomCircle->drawSelf(circleTexId);//绘制底圆
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}

btRigidBody* TexCone::getBody()
{
	return body;//返回刚体指针
}
