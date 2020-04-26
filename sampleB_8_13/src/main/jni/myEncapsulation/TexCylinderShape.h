#ifndef __TexCylinderShape_h
#define __TexCylinderShape_h
#include "TexBody.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/LinearMath/btVector3.h"
#include "../Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "MatrixState.h"

#include "geometry/CylinderSide.h"
#include "geometry/Circle.h"

class TexCylinderShape:public TexBody{

	float halfSize;//立方体的半边长
	btRigidBody *body;//对应的刚体对象

	int texId_bottomCircle;//底部圆形的纹理id
	int texId_topCircle;//顶部圆形的纹理id
	int texId_cylinderSide;//圆柱侧面的纹理id

	btDynamicsWorld *m_dynamicsWorld;//物理世界对象
	float offset_topCircle;//绘制顶圆所需的位移量

public:
	Circle* bottomCircle;//底圆
	CylinderSide* cylinderSide;//侧面

public:
	TexCylinderShape(
			int texId_bottomCircle,//底部圆形的纹理id
			int texId_topCircle,//顶部圆形的纹理id
			int texId_cylinderSide,//圆柱侧面的纹理id
			int n,//切分份数  30
			btVector3 halfExtents,//半区域
			btDynamicsWorld *dynamicsWorld,//物理世界指针
			btScalar mass,//质量
			btVector3 pos,//位置
			btScalar restitutionIn,//恢复系数
			btScalar frictionIn//摩擦系数
			);//初始圆柱侧面平行于y轴，圆形是平行xoz界面上的
	TexCylinderShape(
			int texId_bottomCircleIn,//底部圆形的纹理id
			int texId_topCircleIn,//顶部圆形的纹理id
			int texId_cylinderSideIn,//圆柱侧面的纹理id
			int n,//切分份数
			btVector3 halfExtents//半区域
			);//初始圆柱侧面平行于y轴，圆形是平行xoz界面上的
	void drawSelf();
	void drawSelfNoBody();
	btRigidBody* getBody();
};
#endif
