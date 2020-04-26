#include <math.h>
#include "TexPlane.h"
#include "MatrixState.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

TexPlane::TexPlane(float UNIT_SIZE,int texId,
		btCollisionShape *planeShape,
		btDynamicsWorld *dynamicsWorld,
		btScalar cx,btScalar cy,btScalar cz,
		btScalar restitutionIn,
		btScalar frictionIn)
{
	//创建刚体的初始变换对象
	btTransform groundTransform =  btTransform();
	groundTransform.setIdentity();//设置变换对象的一致性	//变换初始化
	groundTransform.getOrigin().setValue(cx, cy, cz);//设置变换对象的原点
	btVector3 localInertia = btVector3(0, 0, 0);//惯性向量
	//创建刚体的运动状态对象
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	//创建刚体信息对象
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0,myMotionState,planeShape,localInertia);
	rbInfo.m_restitution = restitutionIn;//设置反弹系数
	rbInfo.m_friction = frictionIn;//设置摩擦系数
	//创建刚体
	body = new btRigidBody(rbInfo);
	//将刚体添加进物理世界
	dynamicsWorld->addRigidBody(body);

	this->UNIT_SIZE = UNIT_SIZE;
	this->texId = texId;
	tp = new TexturePlane(
    		1, //半宽
    		1, //
    		0, //位移量
    		UNIT_SIZE//单位长度
			);
}



void TexPlane::drawSelf()
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;
	trans = body->getWorldTransform();
	trans.getOpenGLMatrix(MatrixState::currMatrix);

	MatrixState::pushMatrix();//保护现场
	tp->drawSelf(texId);
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场


}

btRigidBody* TexPlane::getBody()
{
	return body;
}
