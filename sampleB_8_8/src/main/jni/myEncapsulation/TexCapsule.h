#ifndef _TexCapsule_H_
#define _TexCapsule_H_

#include "TexBody.h"
#include "MatrixState.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/LinearMath/btVector3.h"

#include "geometry/Ball.h"
#include "geometry/CylinderSide.h"

class TexCapsule : public TexBody
{
	btRigidBody* body;

	float radius;//半径
	float height;//高度

	int capuleSideTexId;//胶囊侧边纹理id
	int buttomBallTexId;//底部圆纹理id
	int topBallTexId;//顶部圆纹理id

public:
	CylinderSide* capuleSide;//胶囊的侧面
	Ball* bottomBall;//底部圆

public:
	TexCapsule(
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
		);

	TexCapsule(
			float radiusIn,//半径
			float heightIn,//高度
			int capuleSideTexIdIn,//胶囊侧边纹理id
			int buttomBallTexIdIn,//底部圆纹理id
			int topBallTexIdIn,//顶部圆纹理id
			int n//被切分的份数  n
			);
	btRigidBody* getBody();
	void drawSelfNoBody();
	void drawSelf();
};

#endif
