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

	btRigidBody *body;//对应的刚体对象

	int texId_bottomCircle;//底部圆形的纹理id
	int texId_topCircle;//顶部圆形的纹理id
	int texId_cylinderSide;//圆柱侧面的纹理id

	Circle* bottomCircle;//底圆
	CylinderSide* cylinderSide;//侧面
	float offset_topCircle;//绘制顶圆所需的位移量
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
	void drawSelf();//绘制自身刚体的方法
	btRigidBody* getBody();//获取自身刚体的方法
};
#endif
