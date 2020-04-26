#include "TexCuboid.h"

#include "..\Bullet\LinearMath\btVector3.h"
#include "..\Bullet\BulletCollision\CollisionShapes\btCollisionShape.h"
#include "..\Bullet\BulletCollision\CollisionShapes\btBoxShape.h"
#include "..\Bullet\LinearMath\btDefaultMotionState.h"
#include "..\Bullet\BulletDynamics\Dynamics\btRigidBody.h"
#include "MatrixState.h"

TexCuboid::TexCuboid(
	btDynamicsWorld* m_dynamicsWorldIn,//物理世界对象
	btVector3 boxHalfExtentsIn,//长方体的半区域
	btScalar mass,//长方体的质量
	btVector3 pos,//刚体的位置
	btScalar restitutionIn,//恢复系数
	btScalar frictionIn,//摩擦系数
	float UNIT_SIZE,//单位长度
	int frontTexIdIn,//前侧纹理图id
	int backTexIdIn,//后侧纹理图id
	int topTexIdIn,//上侧纹理图id
	int bottomTexIdIn,//下侧纹理图id
	int leftTexIdIn,//左侧纹理图id
	int rightTexIdIn//右侧纹理图id
)
{
	bool isDynamic = (mass != 0);//判断物体是否可以运动
	btVector3 localInertia = btVector3(0, 0, 0);//惯性向量  ineria
	btCollisionShape* colShape = new btBoxShape(boxHalfExtentsIn);
	if(isDynamic)
	{
		colShape->calculateLocalInertia(mass, localInertia);//计算惯性
	}
	btTransform startTansform = btTransform();//创建刚体初始变换对象
	startTansform.setIdentity();//初始化刚体变换对象
	startTansform.getOrigin().setValue(pos.x(), pos.y(), pos.z());//设置变换对象的初始位置

	//创建运动状态对象
	btDefaultMotionState* myMotionState  = new btDefaultMotionState(startTansform,btTransform::getIdentity());

	//刚体信息对象  construction  建设
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	rbInfo.m_restitution = restitutionIn;//设置反弹系数
	rbInfo.m_friction = frictionIn;//设置摩擦系数
	
	body = new btRigidBody(rbInfo);//创建刚体

	m_dynamicsWorldIn->addRigidBody(body);//将刚体添加进物理世界

	this->offset_x = boxHalfExtentsIn.x();
	this->offset_y = boxHalfExtentsIn.y();
	this->offset_z = boxHalfExtentsIn.z();

	//前后的长方形    前位移量 offset_z  后位移量 -offset_z
	frontBackRect = new TextureRect(
    		offset_x, //半宽
    		offset_y, //半高
    		0, //位移量
    		UNIT_SIZE//单位长度
			);
	//上下的长方形   前位移量 offset_y  后位移量 -offset_y
	topBottomRect = new TextureRect(
    		offset_x, //半宽
    		offset_z, //半高
    		0, //位移量
    		UNIT_SIZE//单位长度
			);
	//左右的长方形    前位移量 offset_x  后位移量 -offset_x
	leftRightRect = new TextureRect(
    		offset_z, //半宽
    		offset_y, //半高
    		0, //位移量
    		UNIT_SIZE//单位长度
			);

	this->frontTexId =  frontTexIdIn;//前侧纹理图id
	this->backTexId =  backTexIdIn;//后侧纹理图id
	this->topTexId = topTexIdIn;//上侧纹理图id
	this->bottomTexId = bottomTexIdIn;//下侧纹理图id
	this->leftTexId = leftTexIdIn;//左侧纹理图id
	this->rightTexId = rightTexIdIn;//右侧纹理图id

}

void TexCuboid::drawSelf()
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;
	trans = body->getWorldTransform();
	trans.getOpenGLMatrix(MatrixState::currMatrix);

	if(body->isActive())
	{
		topBottomRect->mFlagK = 0;
		frontBackRect->mFlagK = 0;
		leftRightRect->mFlagK = 0;
	}
	else
	{
		topBottomRect->mFlagK = 1;
		frontBackRect->mFlagK = 1;
		leftRightRect->mFlagK = 1;
	}

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, offset_y, 0);//执行平移
	MatrixState::rotate(90, 1, 0, 0);//执行旋转
	topBottomRect->drawSelf(topTexId);//绘制上面
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, -offset_y, 0);//执行平移
	MatrixState::rotate(-90, 1, 0, 0);//执行旋转
	topBottomRect->drawSelf(bottomTexId);//绘制下面
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(-offset_x, 0, 0);//执行平移
	MatrixState::rotate(90, 0, 1, 0);//执行旋转
	leftRightRect->drawSelf(leftTexId);//绘制左面
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(offset_x, 0, 0);//执行平移
	MatrixState::rotate(-90, 0, 1, 0);//执行旋转
	leftRightRect->drawSelf(rightTexId);//绘制右面
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, 0, offset_z);//执行平移
	frontBackRect->drawSelf(frontTexId);//绘制前面
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, 0, -offset_z);//执行平移
	MatrixState::rotate(180, 0, 1, 0);//执行旋转
	frontBackRect->drawSelf(backTexId);//绘制后面
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}
void TexCuboid::drawSelf1()
{
	MatrixState::pushMatrix();//保护现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, offset_y, 0);//执行平移
	MatrixState::rotate(90, 1, 0, 0);//执行旋转
	topBottomRect->drawSelf(topTexId);//绘制上面
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, -offset_y, 0);//执行平移
	MatrixState::rotate(-90, 1, 0, 0);//执行旋转
	topBottomRect->drawSelf(bottomTexId);//绘制下面
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(-offset_x, 0, 0);//执行平移
	MatrixState::rotate(90, 0, 1, 0);//执行旋转
	leftRightRect->drawSelf(leftTexId);//绘制左面
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(offset_x, 0, 0);//执行平移
	MatrixState::rotate(-90, 0, 1, 0);//执行旋转
	leftRightRect->drawSelf(rightTexId);//绘制右面
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, 0, offset_z);//执行平移
	frontBackRect->drawSelf(frontTexId);//绘制前面
	MatrixState::popMatrix();//恢复现场

	MatrixState::pushMatrix();//保护现场
	MatrixState::translate(0, 0, -offset_z);//执行平移
	MatrixState::rotate(180, 0, 1, 0);//执行旋转
	frontBackRect->drawSelf(backTexId);//绘制后面
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}
btRigidBody* TexCuboid::getBody()
{
	return body;
}
