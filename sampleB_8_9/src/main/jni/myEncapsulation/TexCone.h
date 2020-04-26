#ifndef _TexCone_H_
#define _TexCone_H_

#include "TexBody.h"
#include "MatrixState.h"
#include "../Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "../Bullet/LinearMath/btVector3.h"

#include "geometry/ConeSide.h"
#include "geometry/Circle.h"

class TexCone : public TexBody
{
	btRigidBody* body;
	btDynamicsWorld *m_dynamicsWorld;//物理世界对象

	float radius;//半径
	float height;//高度

	int sideTexId;//圆锥侧边纹理id
	int circleTexId;//圆锥底圆纹理id

	Circle* bottomCircle;//底圆指针
	ConeSide* coneSide;//圆锥侧面

public:
	TexCone(
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
		);
	btRigidBody* getBody();
	void drawSelf();
};

#endif
